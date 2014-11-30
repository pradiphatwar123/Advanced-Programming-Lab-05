
#include<iostream>
#include "globals.h"

int nodetime = 0;
bool cycle;
DFS :: DFS()
{

} 

DFS :: ~DFS()
{

}
/******************************************************************************************************
Verifies Graph for connectedness, presence of cycle
*******************************************************************************************************/
int DFS :: VerifyGraph(GraphAdjListNode **GraphAdjListArr, int no_vertices)
{
		int retval = -1, iter,connected_comps = 0 ;
	
    	/*for(iter = 0; iter < no_vertices; iter++)
		{
           cout << "Graph Node Number is " << (*GraphAdjListArr + iter)->Value << endl;
    	}*/
    
		cycle = false; 
		
    	for(iter = 0; iter < no_vertices; iter++)
		{	              
        	
			if((*GraphAdjListArr + iter)->Color == WHITE)
			{		
						connected_comps++;
						//cout << "Graph Node Number is " << (*GraphAdjListArr + iter)->Value << endl; 			
                        DFS_VISIT(GraphAdjListArr,iter);
			}
      
      	}

		if((connected_comps > 1) && cycle == false) //Not Connected, No Cycle
			retval = 0;
		else if((connected_comps > 1) && cycle == true)// Not Connected, Cycle
			retval = 1;
		else if((connected_comps == 1 ) && cycle == false) // Connected, No Cycle
			retval = 2;
		else if((connected_comps == 1) && cycle == true) //Connected, Cycle
			retval = 3;

      	return retval;     
}


void DFS ::  DFS_VISIT(GraphAdjListNode **GraphAdjListArr,int iter )
{

         nodetime++;

		 (*GraphAdjListArr + iter)->StartTime = nodetime;
         (*GraphAdjListArr + iter)->Color = GRAY;
         GraphAdjNode *AdjNode= (*GraphAdjListArr + iter)->AdjNode;
         while(AdjNode != NULL)
         {
         	if((*GraphAdjListArr + (AdjNode->Value -1 ))->Color == WHITE)
          	{
                	(*GraphAdjListArr + (AdjNode->Value - 1))->Parent =  (*GraphAdjListArr + iter);                                   
                	DFS_VISIT(GraphAdjListArr,(AdjNode->Value - 1));
            }
    
			//Checking For Cycle , Check if v not Parent Node
			if((*GraphAdjListArr + iter)->Parent != (*GraphAdjListArr + (AdjNode->Value -1)))
			{
				if((*GraphAdjListArr + (AdjNode->Value - 1))->StartTime < (*GraphAdjListArr + iter)->StartTime)
				{
					if(cycle != true)
						cycle = true;            
				}
			}
			AdjNode=AdjNode->Next;
         }  
         
         (*GraphAdjListArr + iter)->Color = BLACK;
         nodetime++;
         (*GraphAdjListArr + iter)->FinishTime = nodetime;
		 
}
