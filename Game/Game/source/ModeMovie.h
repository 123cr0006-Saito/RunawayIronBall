//----------------------------------------------------------------------
// @filename ModeMovie.h
// @date: 2024/03/16
// @author: saito ko
// @explanation
// ゲームクリア時にエンディングムービーを再生するクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class ModeMovie : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
protected:
	int _movie;// ムービーハンドル
};

