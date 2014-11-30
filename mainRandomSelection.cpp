/*********************************************************************************************************
Subject           :  Advanced Programming Lab (CS6140)

Assignment        :  05 - Implementing Phase Transitions in Random Graphs

File name         :  main.CPP

Description       :  Implements main functions required to implement Phase Transitions in Random Graphs

Programmers       :  (1) Pradip Hatwar     (CS13M022)
                     (2) Hem Kapil         (CS13M023)
                     (3) Pradeep Vankadaru (CS13M037)

Compiler name     :  g++-4.6

Date created      :  08/11/2013

List of functions : (1)  int 	main(int argc, char *argv[])
		    (2)  int	ValidateCmdline(int, char *[], char **, char **, char **, char **);
		    (3)  int 	CheckFiles(char *, char *,char *,char *);
		    (4)	 int	ValidateFile(char *);	
		    (5)  int 	ValidateInput(char *);
		    (6)  void	PopulateDictionary(char *, char *);
		    (7)  void	LocateInDictionary(char *,char *);
		    (8)  void	ReadInputs(char *, char *);	
		    (9)  int	WriteOutput(char *, long , long);	
                    
History           :  

*********************************************************************************************************/
#include<iostream>
#include<cstdlib>
#include<ctype.h>
#include<malloc.h>
#include<fstream>
#include<string.h>
#include<math.h>
//#include<cstdlib.h>

#include "globals.h"


DFS 			DFSObj;
bool 			CycleFlag,ConnectivityFlag;
GraphAdjListNode	*GraphAdjListArr;

int main(int argc,char *argv[])
{
    
	int	no_vertices, no_graphs;

	//Validation of Commandline may not be required here
	/*if(Validate_Cmdline(argc,argv) == -1)  // Check if proper number of inputs are not given in the command line
	{   
		cout << "Error: valid command for execution is <./a.out> "<< endl;
		cout << "Exiting ....." <<endl;
		exit(-1);		
	}
        else
	{*/	
	ReadInputs(&no_vertices,&no_graphs);
        Construct_RandomGraphs(no_vertices,no_graphs);
	   
     	//}   
	//Freeing the Memory Assigned to Graph Data Structure      
	delete [] GraphAdjListArr;
	return 0;
}
// Reading Inputs
void ReadInputs(int *no_vertices, int *no_graphs)
{
	int 	temp,iter;	
	
	cout << "Enter No of Vertices in the Graph " << endl;
	cin >> temp;
	*no_vertices = temp;	
	cout << "Number of Vertices Entered " << *no_vertices << endl;

	cout << "Enter No of Random Graphs you want to generate " << endl;
	cin >> temp;
	*no_graphs = temp;
	cout << "Number of Graphs Entered " << *no_graphs << endl;

	
	try{
		GraphAdjListArr = new GraphAdjListNode[*no_vertices];
	}catch(bad_alloc xa){		
		cout << "Failed to initialize memory for GraphAdjListArr " << endl; 
	}
	//Initializing Graph Adjacency List		
	for(iter = 0; iter < *no_vertices; iter++)
	{
		GraphAdjListArr[iter].Value = (iter + 1);
		GraphAdjListArr[iter].Color = WHITE;
		GraphAdjListArr[iter].StartTime = 0;
		GraphAdjListArr[iter].FinishTime = 0;
		GraphAdjListArr[iter].Parent = NULL;
		GraphAdjListArr[iter].AdjNode =  NULL;		
		cout << "Node Number Assigned is " << GraphAdjListArr[iter].Value << endl;
	}

}
void Construct_RandomGraphs(int no_vertices, int no_graphs)
{
	int 	cycle_graphs_count,connected_graphs_count,retval,iter,retval1;
	long	exp_no_edges,max_no_edges;
	float	threshold,cycle_graphs_ratio,connected_graphs_ratio,c;
	char	exit_char;
	
	//Max Number of Edges Possible with the given no of vertices
	max_no_edges = (((no_vertices)*(no_vertices - 1)) / 2); //Formula: select any 2 vertices 1 edge is generated 
	//Temporarily Threshold is taken as 1/n
	threshold = ((float)1/(no_vertices));
	cout << "Threshold value is " << threshold << endl;


	while(true)
	{
		cout << "Enter threshold function coefficient (c value)" << endl;
		cin >> c;
		cout << "Threshold function coefficient entered is " << c << endl;
		if((threshold*c) > 1)
		{
			cout << "Threshold coefficient is invalid,exiting the program" << endl;
			break;
		}
		//Expected Number of Edges Possible with the given no of vertices, Threshold
		exp_no_edges = (int)(max_no_edges*threshold*c);
		for(iter = 0; iter < no_graphs; iter++)
		{
			Generate_Graph(no_vertices,threshold,c);//It generates graph using Threshold Funcion
			//Calling DFS to check for Graph Connectivity and Cycle Presence
			retval = DFSObj.VerifyGraph(&GraphAdjListArr,no_vertices);	
			//No Cycle, Not Connected
			if(retval == 0)	
			{

			}
			//Cycle, Not Connected
			else if(retval == 1)	
			{
				cycle_graphs_count++;
			}
			//No Cycle, Connected
			else if(retval == 2)	
			{
				connected_graphs_count++;
			}
			//Cycle, Connected
			else if(retval == 3)	
			{
				cycle_graphs_count++;
				connected_graphs_count++;
			}				
		}
		cycle_graphs_ratio = (float)((cycle_graphs_count) / (no_graphs));
		connected_graphs_ratio = (float) ((connected_graphs_count) / (no_graphs));
         	retval1 = WriteOutput(threshold,cycle_graphs_ratio,connected_graphs_ratio);
		cout << "Do you want to coninue,Press 'y' to continue, 'n' to exit " << endl;
		cin >> exit_char;
		if(exit_char == 'n')
			break;
	}	
}
//It generates graph using Threshold Funcion
void Generate_Graph(int no_vertices,float threshold,float c)
{
	
	int num_edges = 0, i=0, j=0;
	float random_num = 0.0;

	num_edges = (int)(no_vertices * (no_vertices -1))/2 ;
	
	for(i=1; i<no_vertices; i++)
	{
		for(j=i+1; j<= no_vertices; j++)
		{

			// probability of existence of edge between i and j
			float h;
				h=rand();
			random_num = h/(RAND_MAX+1);

		//	cout << " rand num gen is " << random_num << endl;

			if(random_num < threshold*c)
				cout << "There is an edge at "<<i << " and " <<j<<endl;;

			
		}



	}
	




}
/*********************************************************************************************************
Function Name            : WriteOutput
Inputs                   : (1) Reference to Transition Function Threshold
			   (2) Ratio of Graphs containing cycle
			   (3) Ratio of Graphs connected
Outputs                  : -1 In case of any error occurrs
                            0 In case of sucessful write
Description              :  This function writes Probabilities information into 2 output files
*********************************************************************************************************/
int WriteOutput(float threshold, float cycle_graphs_ratio, float connected_graphs_ratio)
{
	char		cyclefile[100],connectedfile[100];
	int 		retval = 0,p1 = 0, no_chars = 0;
	ofstream 	optstrm;
	char		buff[100];
	size_t 		pos=-1;


	strcpy(cyclefile,"cycle.txt");
	strcpy(connectedfile,"connected.txt");

	cout << "Cycle Output File is " << cyclefile << endl;
	cout << "Connected Output File is " << connectedfile << endl;

	optstrm.open(cyclefile, ios::app); /* Opening cycle file in Append Mode*/

	if(!optstrm)
	{
		cout << "Failed to open File " << cyclefile << endl;		
		retval = -1;
	}
	else
	{			
		//writing value to the output file
		no_chars = sprintf(buff,"%f",threshold);
		buff[no_chars] = '\0';
		if(optstrm.write(buff,no_chars))
		{
			//For writing blank space after every value is written to the file
			buff[0] = ' ';
			no_chars = 1;
			buff[no_chars] = '\0';
			if(optstrm.write(buff,no_chars))
			{
				no_chars = sprintf(buff,"%f",cycle_graphs_ratio);
				buff[no_chars] = '\0';
				if(optstrm.write(buff,no_chars))
				{
					//For writing blank space after every value is written to the file
					buff[0] = '\n';
					no_chars = 1;
					buff[no_chars] = '\0';
					if(!optstrm.write(buff,no_chars))
					{
						retval = -1;					

					}					
				}
				else
				{
					retval = -1;
				}				
			}
			else
			{
				retval = -1;	
				
			}
		}
		else
		{
			retval = -1;
		}
		optstrm.close();	// Closing output file 	
	}	

	optstrm.open(connectedfile, ios::app); /* Opening connected file in Append Mode*/

	if(!optstrm)
	{
		cout << "Failed to open File " << connectedfile << endl;		
		retval = -1;
	}
	else
	{			
		//writing value to the output file
		no_chars = sprintf(buff,"%f",threshold);
		buff[no_chars] = '\0';
		if(optstrm.write(buff,no_chars))
		{
			//For writing blank space after every value is written to the file
			buff[0] = ' ';
			no_chars = 1;
			buff[no_chars] = '\0';
			if(optstrm.write(buff,no_chars))
			{
				no_chars = sprintf(buff,"%f",connected_graphs_ratio);
				buff[no_chars] = '\0';
				if(optstrm.write(buff,no_chars))
				{
					//For writing blank space after every value is written to the file
					buff[0] = '\n';
					no_chars = 1;
					buff[no_chars] = '\0';
					if(!optstrm.write(buff,no_chars))
					{
						retval = -1;					

					}					
				}
				else
				{
					retval = -1;
				}				
			}
			else
			{
				retval = -1;	
				
			}
		}
		else
		{
			retval = -1;
		}
		optstrm.close();	// Closing output file 	
	}
	return retval;	
}
/******************************************************************************************************

command validation

*******************************************************************************************************/
int Validate_Cmdline(int argc,char *argv[])
{
	int 	error=0;
	
	if(argc < 1)	
	{	
		cout << "Error: valid command for execution is " << endl;
		cout << "<exename> "<< endl;		
		error = -1;
	}		
	else
	{
		
	}
	return error;        
}
	

