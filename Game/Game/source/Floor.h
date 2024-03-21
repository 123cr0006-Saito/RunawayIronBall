#pragma once
#include "myJson.h"
#include "appframe.h"
class Floor
{
public :
	Floor();
	~Floor();
	void Delete();
	void Create(myJson jsonFile, int stageNum);
	std::vector<std::string> LoadName(int stageNum);
	void Render();
protected:
	std::vector<int> _modelList;
	std::vector<std::string> _nameList;
};

