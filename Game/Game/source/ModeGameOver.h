#pragma once
#include "appframe.h"
#include "ModeGame.h"
class ModeGameOver : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
protected:
	XInput* _input;
	ModeGame* _mode;
	std::unordered_map<std::string,int> _handle;
	int _selectItem;

	int _model;
	int _modelFrame;
	VECTOR _targetPos;
	VECTOR _cameraPos;
};

