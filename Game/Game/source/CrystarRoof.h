#pragma once
#include "DxLib.h"
class CrystarRoof
{
public:
	CrystarRoof(int model, int parent);
	~CrystarRoof();
	bool Updata();
	bool Render();
protected:
	int _model;
	int _frame;
	int _parent;
};

