//----------------------------------------------------------------------
// @filename ModeTutorial.h
// ＠date: 2024/03/17
// ＠author: saito ko
// @explanation
// ステージの最初にチュートリアル画面を表示するクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class ModeTutorial : public ModeBase
{
	typedef ModeBase base;
public:
	ModeTutorial(int* handle,int size);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
protected:
	XInput* _input;
	int* _handle;
	int _pageSize;

	int _selectItem;
};