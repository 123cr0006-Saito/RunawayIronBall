#pragma once
#include "appframe.h"
#include "ModeGame.h"
#include "ModeBossBattle.h"
class ModeGameOver : public ModeBase
{
	typedef ModeBase base;
public:
	ModeGameOver(ModeGame* mode = nullptr);
	ModeGameOver(ModeBossBattle* mode = nullptr);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
	void AnimProcess();
	void SelectProcess();
protected:
	XInput* _input;
	ModeGame* _modeGame;
	ModeBossBattle* _modeBossBattle;
	std::unordered_map<std::string,int> _handle;
	int _selectItem;

	bool _selectEnd;
	int _model;
	int _modelFrame;
	VECTOR _targetPos;
	VECTOR _cameraPos;
};

