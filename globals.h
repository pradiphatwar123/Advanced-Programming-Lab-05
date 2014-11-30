#ifndef _globals_H_
#define _globals_H_

#define POSITIVE_SENTINEL ((pow(2,((sizeof(int)*8)-1))) - 1)

#define NEGATIVE_SENTINEL (-(pow(2,((sizeof(int)*8)-1)))) 


using namespace std;

typedef enum {WHITE,GRAY,BLACK} NodeColor;

//Graph Adjacency List Node Structure 

class GraphAdjNode	
{
	public:
		int 		Value;		
		GraphAdjNode	*Next;	
};

//Graph Node Structure 

class GraphAdjListNode	
{
	public:
		int 			Value;
		NodeColor		Color;
		int			StartTime;
		int			FinishTime;
		GraphAdjListNode	*Parent;
		GraphAdjNode		*AdjNode;	
};

/*****************************************************************************************************
DFS class Verifying Cycle, Connectedness in a Graph
*****************************************************************************************************/
class DFS
{
      public:
              DFS(); 
              ~DFS(); 
				     
	      int VerifyGraph(GraphAdjListNode **, int );	
	      void DFS_VISIT(GraphAdjListNode **, int );		 					
};


//int 	Validate_Cmdline(int, char *[]);
void 	ReadInputs(int *, int *, int *);
void	Construct_RandomGraphs(int, int, int);
void 	Generate_Graph(int ,float ,float);
int 	WriteOutput(float , float, float, int);
void	GeneratePlt(int);
#endif
