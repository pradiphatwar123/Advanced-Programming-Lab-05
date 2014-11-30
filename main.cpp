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
#include<time.h>


#include "globals.h"


DFS 			DFSObj; //Object for Depth First Search Traversal
bool 			CycleFlag,ConnectivityFlag; // Flags to indicate existence of cycles and connectedness in graphs
GraphAdjListNode	*GraphAdjListArr; // Adjacency List Array
GraphAdjNode 		*adjnode,*adjnode1; // Adjacency List Node


/******************************************************************************************************
Main function 
*******************************************************************************************************/

int main(int argc,char *argv[])
{
    
	int	no_vertices, no_graphs;
	int prob_choice;

	
	srand(time(0)); // Seeding the random function

	ReadInputs(&no_vertices,&no_graphs,&prob_choice); // Read inputs from user
    Construct_RandomGraphs(no_vertices,no_graphs, prob_choice); // Contruct graphs as per the inputs
  
	delete [] GraphAdjListArr;
	cout << "Created connected and cycle Output Files " << endl;
	
	GeneratePlt(prob_choice);
	return 0;
}


/******************************************************************************************************
This function reads Inputs from user
*******************************************************************************************************/

void ReadInputs(int *no_vertices, int *no_graphs, int *choice)
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

	do
	{
		cout << "Select the Probability Function "<< endl;
		cout << "1. 1/n "<< endl;
		cout << "2. 1/(n^2) " << endl;
		cout << "3. log(n)/n " << endl;
		cin >> temp;
		if(cin.fail())
		{
			cout << "Invalid choice,Exiting the program" << endl;	 
			break;
		}
		*choice = temp;
		
	}while(temp > 3);

	//Creating Memory for Adjacency List
	try{
		GraphAdjListArr = new GraphAdjListNode[*no_vertices];
	}catch(bad_alloc xa){		
		cout << "Failed to initialize memory for GraphAdjListArr " << endl; 
	}	
}


/******************************************************************************************************
This function constructs random graphs
*******************************************************************************************************/

void Construct_RandomGraphs(int no_vertices, int no_graphs, int choice)
{
	int 		cycle_graphs_count,connected_graphs_count,retval,iter,iter1,retval1;
	long		exp_no_edges;
	float		threshold,cycle_graphs_ratio,connected_graphs_ratio,c;
	char		exit_char;
	GraphAdjNode 	*temp;

	switch(choice)
	{
		case 1: 	
			threshold = ((float)1/(no_vertices));  // Probability function 1/n
			break;
		case 2: 	
			threshold = ((float)1/(no_vertices * no_vertices)); // Probability function 1/n^2
			break;
		case 3: 	
			threshold = ((float)(log(no_vertices))/(no_vertices)); //// Probability function log(n)/n
			break;
		default:
			break;
	}

	c = 1; // Initial value of constant c. To be increased till probability reaches 1
	while(true)
	{
	
		if((threshold*c) > 1)
		{
			break;
		}
	

		cycle_graphs_count = 0, connected_graphs_count = 0; //Initialise counts for no. of graphs with cycle and conncted for
															//each iteration
		for(iter = 0; iter < no_graphs; iter++)
		{
		
			Generate_Graph(no_vertices,threshold,c);//It generates graph using Threshold Funcion
		
			//Calling DFS to check for Graph Connectivity and Cycle Presence
			retval = DFSObj.VerifyGraph(&GraphAdjListArr,no_vertices);	
		

			//No Cycle, Not Connected
			if(retval == 0)	
			{
			//	cout << "No Cycle, Not Connected " << endl;
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
			//Deleting Adjacency Lists
			for(iter1 = 0; iter1 < no_vertices; iter1++)
			{	
	
			
				temp = GraphAdjListArr[iter1].AdjNode;
			
				//if(temp != NULL)	
				//	cout << "temp points to " << temp->Value;
				while(temp != NULL)
				{
					GraphAdjListArr[iter1].AdjNode = temp->Next;			
					delete temp;	
					temp = GraphAdjListArr[iter1].AdjNode;
				}
				
			}
			
			
			//Testing Purpose Check for linked list updation
			/*cout << "Checking Linked List Emptied "<< endl;
			for(iter1 = 0; iter1 < no_vertices; iter1++)
			{
	
				cout <<	GraphAdjListArr[iter1].Value;
				temp = GraphAdjListArr[iter1].AdjNode;
				while(temp != NULL)
				{
					cout << " --> " << temp->Value;			
					temp = temp->Next;
				}
				cout << endl;
			}*/
			
			//cout << "End of For Loop" <<  endl;
		}


		// Calculate Number_of_graphs_with_cycle / total_number_of_graphs_plotted
		cycle_graphs_ratio = ((float)(cycle_graphs_count) / (no_graphs));

		// Calculate Number_of_connected_graphs / total_number_of_graphs_plotted
		connected_graphs_ratio = ((float)(connected_graphs_count) / (no_graphs));

		// Update the graphs
        retval1 = WriteOutput((c*threshold),cycle_graphs_ratio,connected_graphs_ratio,choice);

		//cout << "Do you want to coninue,Press 'y' to continue, 'n' to exit " << endl;
	
		c += 1;
		//if(exit_char == 'n')*/
		//	break;
	
	}	
}
//It generates graph using Threshold Funcion
void Generate_Graph(int no_vertices,float threshold,float c)
{
	
	int 		num_edges = 0, i=0, j=0, edge_count=0, iter=0;
	float 		random_num = 0.0;
	bool 		error;	
	GraphAdjNode 	*temp;
	float 		h;
	
	//Initializing Graph Adjacency List		
	for(iter = 0; iter < no_vertices; iter++)
	{
		GraphAdjListArr[iter].Value = (iter + 1);
		GraphAdjListArr[iter].Color = WHITE;
		GraphAdjListArr[iter].StartTime = 0;
		GraphAdjListArr[iter].FinishTime = 0;
		GraphAdjListArr[iter].Parent = NULL;
		GraphAdjListArr[iter].AdjNode =  NULL;		
	}
	

	num_edges = (int)(no_vertices * (no_vertices -1))/2 ;	
	//Randomly Generating Edges of the Graph
	for(i=1; i < no_vertices; i++)
	{
		for(j=i+1; j<= no_vertices; j++)
		{

		// probability of existence of edge between i and j

				
			h = rand();
			random_num = h/(float)(RAND_MAX+1);

			if(random_num < threshold*c)
			{
				edge_count++;

				try{
	    				adjnode = new GraphAdjNode;
    				}catch (bad_alloc xa) {    
					error = true;    	
        				cout << "Failed to Intialize Node of adjacency list" << endl;           
    				}
				if(!error)
				{

					adjnode->Value = j;
					adjnode->Next = NULL;
				}
				
				temp = GraphAdjListArr[i-1].AdjNode;
				if(temp != NULL)
                		{ 
					while(temp->Next != NULL)
					{
						temp = temp->Next;	
			
					}// found last node
					temp->Next = adjnode;
				}
				else{
					GraphAdjListArr[i-1].AdjNode = adjnode;	
				}
					
				//Creating NewNode 	j->i
				try{
	    				adjnode = new GraphAdjNode;
    				}catch (bad_alloc xa) {    
					error = true;    	
        				cout << "Failed to Intialize Node of adjacency list" << endl;           
    				}
				if(!error)
				{

					adjnode->Value = i;
					adjnode->Next = NULL;
				}
				
				temp = GraphAdjListArr[j-1].AdjNode;
				if(temp != NULL)
                		{ 
					while(temp->Next != NULL)
					{
						temp = temp->Next;	
			
					}// found last node
					temp->Next = adjnode;
				}
				else{
					GraphAdjListArr[j-1].AdjNode = adjnode;	
				}
					
			}

			
		}



	}
	// Check for linked list updation
	/*for(iter = 0; iter < no_vertices; iter++)
	{
	
		cout <<	GraphAdjListArr[iter].Value;
		temp = GraphAdjListArr[iter].AdjNode;
		while(temp != NULL)
		{
			cout << " --> " << temp->Value;			
			temp = temp->Next;
		}
		cout << endl;
	
	}*/

	

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
int WriteOutput(float threshold, float cycle_graphs_ratio, float connected_graphs_ratio, int choice)
{
	char		cyclefile[100],connectedfile[100];
	int 		retval = 0,p1 = 0, no_chars = 0;
	ofstream 	optstrm;
	char		buff[100];
	size_t 		pos=-1;
	char		temp[10];
	static	int	write_count = 0;

	sprintf(temp,"%d",choice);
	strcpy(cyclefile,"cycle_");
	strcpy(connectedfile,"connected_");

	strcat(cyclefile,temp);
	strcat(cyclefile,".txt");

	strcat(connectedfile,temp);
	strcat(connectedfile,".txt");

	//cout << "Cycle File "<< cyclefile << endl;
    //cout << "Connected File " << connectedfile << endl;
	if(write_count == 0)
		optstrm.open(cyclefile, ios::out | ios::trunc); /* First time Opening cycle file in Write Mode*/
	else
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

	if(write_count == 0)
		optstrm.open(connectedfile, ios::out | ios::trunc); /* First time Opening connected file in Write Mode*/
	else
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
	write_count++;

	return retval;	
}
/*********************************************************************************************************
Function Name            : GeneratePlt
Inputs                   : (1) Reference to Transition Function Threshold
			   (2) Ratio of Graphs containing cycle
			   (3) Ratio of Graphs connected
Outputs                  : -1 In case of any error occurrs
                            0 In case of sucessful write
Description              :  This function writes Probabilities information into 2 output files
*********************************************************************************************************/
void GeneratePlt(int choice)
{
	char		cyclefile[100],connectedfile[100];
	int 		retval = 0,p1 = 0, no_chars = 0;
	ofstream 	optstrm;
	char		buff[1000];
	size_t 		pos=-1;
	char		temp[10];
	char		conn_lbl[100];
	char		cycle_lbl[100];
	int			length;	


	sprintf(temp,"%d",choice);
	strcpy(cyclefile,"cycle_");
	strcpy(connectedfile,"connected_");

	strcat(cyclefile,temp);
	strcat(cyclefile,".plt");

	strcat(connectedfile,temp);
	strcat(connectedfile,".plt");

	cout << "Cycle File "<< cyclefile << endl;
    cout << "Connected File " << connectedfile << endl;

	strcpy(conn_lbl,"\"Connectedness\" ");
	
	optstrm.open(connectedfile, ios::out | ios::trunc); /*Opening connected plt file in Write Mode*/
	
	if(!optstrm)
	{
		cout << "Failed to open File " << connectedfile << endl;		
		retval = -1;
	}
	else
	{					
		//writing value to the output file
		strcpy(buff,"set xrange[0:1.108011]");
		no_chars = strlen("set xrange[0:1.108011]");

		strcat(buff,"\n");
		no_chars++;

		strcat(buff,"set title ");
		no_chars += strlen("set title");
		strcat(buff,conn_lbl);	
		no_chars += strlen(conn_lbl);
		buff[no_chars] = '\0';
		if(optstrm.write(buff,no_chars))
		{
			//For writing blank space after every value is written to the file
			buff[0] = '\n';
			no_chars = 1;
			buff[no_chars] = '\0';
			if(optstrm.write(buff,no_chars))
			{
				strcpy(buff,"set xlabel ");
				no_chars = strlen("set xlabel");
				strcpy(conn_lbl,"\"c*");
				strcat(buff,conn_lbl);
				no_chars += strlen(conn_lbl);
				if(choice == 1)
				{
					strcpy(conn_lbl,"1/n\" ");
					strcat(buff,conn_lbl);
					no_chars += strlen(conn_lbl);
				}
				else if(choice == 2)
				{
					strcpy(conn_lbl,"1/(n^2)\" ");
					strcat(buff,conn_lbl);
					no_chars += strlen(conn_lbl);
				}
				else
				{
					strcpy(conn_lbl,"(long)/n\" ");
					strcat(buff,conn_lbl);
					no_chars += strlen(conn_lbl);
				}
				buff[no_chars] = '\0';
				if(optstrm.write(buff,no_chars))
				{
					//For writing blank space after every value is written to the file
					buff[0] = '\n';
					no_chars = 1;
					buff[no_chars] = '\0';
					if(optstrm.write(buff,no_chars))
					{
						strcpy(conn_lbl,"set ylabel \"P(Conected)\" ");
						no_chars = strlen(conn_lbl);
						strcpy(buff,conn_lbl);
						strcat(buff,"\n");
						no_chars++;
						strcpy(conn_lbl,"set yrange[0:1.2]");								
						no_chars += strlen(conn_lbl);
						strcat(buff,conn_lbl);
						buff[no_chars] = '\0';
						if(optstrm.write(buff,no_chars))
						{
							//For writing blank space after every value is written to the file
							buff[0] = '\n';
							no_chars = 1;
							buff[no_chars] = '\0';
							if(optstrm.write(buff,no_chars))
							{
								
								strcpy(conn_lbl,"plot \"connected_");
								no_chars = strlen(conn_lbl);
								strcpy(buff,conn_lbl);
								
								strcat(buff,temp);	
								no_chars++;

								//strcpy(conn_lbl,".txt\" title \" with lines, 'connected_");
								strcpy(conn_lbl,".txt\" with lines");
								no_chars += strlen(conn_lbl);
								strcat(buff,conn_lbl);

								/*strcat(buff,temp);	
								no_chars++;
								strcpy(conn_lbl,".txt' using 3: 4 title \"with lines lc rgb 'black'");
								no_chars += strlen(conn_lbl);
								strcat(buff,conn_lbl);
*/
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

	strcpy(conn_lbl,"\"Cycle\" ");
	
	optstrm.open(cyclefile, ios::out | ios::trunc); /*Opening connected plt file in Write Mode*/
	
	if(!optstrm)
	{
		cout << "Failed to open File " << connectedfile << endl;		
		retval = -1;
	}
	else
	{					
		//writing value to the output file
		strcpy(buff,"set xrange[0:1.108011]");
		no_chars = strlen("set xrange[0:1.108011]");

		strcat(buff,"\n");
		no_chars++;

		strcat(buff,"set title ");
		no_chars += strlen("set title");
		strcat(buff,conn_lbl);	
		no_chars += strlen(conn_lbl);
		buff[no_chars] = '\0';
		if(optstrm.write(buff,no_chars))
		{
			//For writing blank space after every value is written to the file
			buff[0] = '\n';
			no_chars = 1;
			buff[no_chars] = '\0';
			if(optstrm.write(buff,no_chars))
			{
				strcpy(buff,"set xlabel ");
				no_chars = strlen("set xlabel");
				strcpy(conn_lbl,"\"c*");
				strcat(buff,conn_lbl);
				no_chars += strlen(conn_lbl);
				if(choice == 1)
				{
					strcpy(conn_lbl,"1/n\" ");
					strcat(buff,conn_lbl);
					no_chars += strlen(conn_lbl);
				}
				else if(choice == 2)
				{
					strcpy(conn_lbl,"1/(n^2)\" ");
					strcat(buff,conn_lbl);
					no_chars += strlen(conn_lbl);
				}
				else
				{
					strcpy(conn_lbl,"(long)/n\" ");
					strcat(buff,conn_lbl);
					no_chars += strlen(conn_lbl);
				}
				buff[no_chars] = '\0';
				if(optstrm.write(buff,no_chars))
				{
					//For writing blank space after every value is written to the file
					buff[0] = '\n';
					no_chars = 1;
					buff[no_chars] = '\0';
					if(optstrm.write(buff,no_chars))
					{
						strcpy(conn_lbl,"set ylabel \"P(Cycle)\" ");
						no_chars = strlen(conn_lbl);
						strcpy(buff,conn_lbl);
						strcat(buff,"\n");
						no_chars++;
						strcpy(conn_lbl,"set yrange[0:1.2]");								
						no_chars += strlen(conn_lbl);
						strcat(buff,conn_lbl);
						buff[no_chars] = '\0';
						if(optstrm.write(buff,no_chars))
						{
							//For writing blank space after every value is written to the file
							buff[0] = '\n';
							no_chars = 1;
							buff[no_chars] = '\0';
							if(optstrm.write(buff,no_chars))
							{
								
								strcpy(conn_lbl,"plot \"cycle_");
								no_chars = strlen(conn_lbl);
								strcpy(buff,conn_lbl);
								
								strcat(buff,temp);	
								no_chars++;

								//strcpy(conn_lbl,".txt\" title \" with lines, 'connected_");
								strcpy(conn_lbl,".txt\" with lines");
								no_chars += strlen(conn_lbl);
								strcat(buff,conn_lbl);

								/*strcat(buff,temp);	
								no_chars++;
								strcpy(conn_lbl,".txt' using 3: 4 title \"with lines lc rgb 'black'");
								no_chars += strlen(conn_lbl);
								strcat(buff,conn_lbl);
*/
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
	

