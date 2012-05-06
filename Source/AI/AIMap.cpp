#include "AIMap.h"


AIMap::AIMap()
{
	this->mGridSize = 0;
	this->mNodes = NULL;
}
AIMap::~AIMap()
{
	if(this->mNodes)
	{
		while(this->mNodes->size() > 0)
		{
			this->mNodes->getAndRemove(0);
		}
	}
}

bool AIMap::LoadFromFile(string fileName)
{
	ifstream in;
	in.open(fileName);

	float tmp = 0.0f;
	char buffer[3];
	string line = "";
	if(in)
	{
		//grid size (functions do not support ',' (C#))
		in.read(buffer, 3);
		line = buffer[0];
		sscanf_s(line.c_str(), "%f", &tmp);
		this->mGridSize = tmp;
		line = buffer[2];
		sscanf_s(line.c_str(), "%f", &tmp);
		this->mGridSize += tmp * 0.1f;
		std::getline(in, line); //move to next line

		//nr of areas
		unsigned int nrOfAreas = 0;
		std::getline(in, line);
		sscanf_s(line.c_str(), "%d", &nrOfAreas);
		AIArea** areas = new AIArea*[nrOfAreas];
		string type = "";
		bool forbidden = false;
		float x = 0.0f;
		float z = 0.0f;
		for(unsigned int i = 0; i < nrOfAreas; i++)
		{
			areas[i] = NULL;
			std::getline(in, type); //type
			std::getline(in, line);	//forbidden
			forbidden = false; //reset
			if(line == "True") forbidden = true;
			std::getline(in, line); //x
			sscanf_s(line.c_str(), "%f", &x);
			std::getline(in, line); //z
			sscanf_s(line.c_str(), "%f", &z);

			if(type == "Rectangle")
			{
				float width = 0.0f;
				float height = 0.0f;
				std::getline(in, line); //width
				sscanf_s(line.c_str(), "%f", &width);
				std::getline(in, line); //height
				sscanf_s(line.c_str(), "%f", &height);

				areas[i] = new AIRectangle(forbidden, x, z, width, height);
			}
			else if(type == "Circle")
			{
				float radius = 0.0f;
				std::getline(in, line); //radius
				sscanf_s(line.c_str(), "%f", &radius);

				areas[i] = new AICircle(forbidden, x, z, radius);
			}
		}
		
		//**todo: convert to nodes**


		//**todo: convert from pixel size to world size**
		//this->mNodes->

		for(unsigned int i = 0; i < nrOfAreas; i++)
		{
			SAFE_DELETE(areas[i]);
		}
		SAFE_DELETE_ARRAY(areas);

		return true;
	}
	else
	{
		return false;
	}
}