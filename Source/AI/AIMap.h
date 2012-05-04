//Written by Markus Tillman.

/*
	Class containing map/grid for the AI game world.
*/

#pragma once

#include "stdafx.h"

class AIMap
{
	private:
		class Node
		{
			private:
				float x;
				float y;
				float value;

			public:
				Node()								{ this->x = 0.0f; this->y = 0.0f; this->value = 0.0f; }
				Node(float x, float y, float value) { this->x = x; this->y = y; this->value = value; }
		};

	private:
		float mGridSize;

	public:
		AIMap();
		virtual ~AIMap();

		bool LoadFromFile(string fileName);
};
