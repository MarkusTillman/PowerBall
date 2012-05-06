//Written by Markus Tillman.

/*
	Class containing map/grid for the AI game world.
*/

#pragma once

#include "stdafx.h"
#include "AIRectangle.h"
#include "AICircle.h"

class AIMap
{
	private:
		class Node
		{
			private:
				float x; //center
				float z; //center
				float value;

			public:
				Node()								{ this->x = 0.0f; this->z = 0.0f; this->value = 0.0f; }
				Node(float x, float z, float value) { this->x = x; this->z = z; this->value = value; }
		};

	private:
		float					mGridSize; //grid size in world coordinates
		MaloW::Array<Node*>*	mNodes;

	public:
		AIMap();
		virtual ~AIMap();

		bool LoadFromFile(string fileName);
};
