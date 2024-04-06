//----------------------------------------------------------------------
// @filename ModeGameOver.h
// ＠date: 2024/02/15
// ＠author: saito ko
// @explanation
// ゲームオーバー時に遷移し、リトライかタイトルに戻るかを選択するモード
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class ModeGameOver : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
	void AnimProcess();
	void SelectProcess();
protected:
	XInput* _input;
	std::unordered_map<std::string,int> _handle;
	int _selectItem;

	bool _selectEnd;
	int _model;
	int _modelFrame;
	VECTOR _targetPos;
	VECTOR _cameraPos;
};

