#pragma once
#include "DxLib.h"
#include "appframe.h"
class CrystarRoof
{
public:
	CrystarRoof(int model, int parent,std::string frameName);
	~CrystarRoof();
	bool Update();
	bool Render();
protected:
	int _model;
	int _frame;
	int _parent;
	std::string _joint;
};

