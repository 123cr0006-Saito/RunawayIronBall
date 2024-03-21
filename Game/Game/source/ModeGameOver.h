#pragma once
#include "appframe.h"
#include "ModeGame.h"
class ModeGameOver : public ModeBase
{
	typedef ModeBase base;
public:
	ModeGameOver(ModeGame* mode = nullptr);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
	void AnimProcess();
	void SelectProcess();
protected:
	XInput* _input;
	ModeGame* _mode;
	std::unordered_map<std::string,int> _handle;
	int _selectItem;

	bool _selectEnd;
	int _model;
	int _modelFrame;
	VECTOR _targetPos;
	VECTOR _cameraPos;
};

