#include "AStarSearch.h"

//private


/**
Node* ReconstructPath(MaloW::Array<Node*>* visitedList, Node* currentNode)
{
	if(visitedList->search(currentNode))
	{
		Node* p = ReconstructPath(visitedList, currentNode);
        
		//return (p + currentNode);**
		return NULL;
	}
    else
	{
         return currentNode;
	}
}


//public
AStarSearch::AStarSearch()
{

}
AStarSearch::~AStarSearch()
{

}
		
		
MaloW::Array<Node>* AStarSearch::GetPath(Node* start, Node* goal)
{
	MaloW::Array<Node*> closedList; //nodes already evaluated
	MaloW::Array<Node*> openList;	//tentatives nodes to be evaluated (starting with the start node)
	MaloW::Array<Node*> visitedList;//nodes already visited

	openList.add(start);
	//**add grid**

	const int size = 512;//**
	int gScore[size];
	int hScore[size];
	int fScore[size];

	gScore[0] = 0;	//cost from start along best known path
	hScore[0] = -1; //heuristic_cost_estimate(start, goal)**
	fScore[0] = gScore[0] + hScore[0]; //estimated cost from start to goal

	while(!openList.isEmpty())
	{
		Node currentNode; //= the node in openset having the lowest f_score[] value**
		if(&currentNode == goal) //**pekare eller värden?**
		{
			//return reconstruct_path(came_from, goal)**
		}

		closedList.add(&currentNode);
		openList.remove(&currentNode);
		/*
		for each neighbour in neight_nodes(current)
		{
			if(neighbor in closedset)
			{
				continue;
			}
			tentative_g_score := g_score[current] + dist_between(current,neighbor)
			
			if neighbor not in openset
                add neighbor to openset
                h_score[neighbor] := heuristic_cost_estimate(neighbor, goal)
                tentative_is_better := true
            else if tentative_g_score < g_score[neighbor]
                tentative_is_better := true
			else
				tentative_is_better := false
 
            if tentative_is_better = true
                came_from[neighbor] := current
                g_score[neighbor] := tentative_g_score
                f_score[neighbor] := g_score[neighbor] + h_score[neighbor]
 
		}
		
	}
    //return failure

	return NULL;
}

*/