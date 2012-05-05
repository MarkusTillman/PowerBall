#include "AIMap.h"


AIMap::AIMap()
{
	this->mGridSize = 0;
	this->mNodes = NULL;
}
AIMap::~AIMap()
{
	while(this->mNodes->size() > 0)
	{
		this->mNodes->getAndRemove(0);
	}
}

bool AIMap::LoadFromFile(string fileName)
{
	ifstream in;
	in.open(fileName);

	if(in)
	{


		return true;
	}
	else
	{
		return false;
	}
}