//Written by Markus Tillman.

/*
	Class for pathing using A*-search algorithm.
	**OBS! LNK2019-fel med Node-klassen


#pragma once

#include "stdafx.h"

class Node
{
	private:
		int nX;
		int nY;
		int nValue;

	public:
		Node() { nX = -1; nY = -1; nValue = 0; }
		Node(int x, int y, int value) { nX = x; nY = y; nValue = value; }
		virtual ~Node();
};

class AStarSearch
{

	private:
		//**
		Node* ReconstructPath(MaloW::Array<Node*>* visitedList, Node* currentNode);


		MaloW::Array<Node>* GetQuickPath(Node start, Node goal); //private, return?**
		MaloW::Array<Node>* GetFullPath(); //return, notify?**
		void GetSplicePath(); //private, return?**

		//distance-plus-cost heruistic F(x)
		//		path-cost function G(x)
		//		admissible "heuristic estimate" of the distance to the goal H(x)

	public:
		AStarSearch();
		virtual ~AStarSearch();
		
		//open list - priority queue**
		//dynamic objects**
		MaloW::Array<Node>* GetPath(Node* start, Node* goal);
};*/