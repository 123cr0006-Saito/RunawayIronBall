//----------------------------------------------------------------------
// @filename ModeLoading.h
// ＠date: 2024/03/06
// ＠author: saito ko
// @explanation
// loading時のキャラクターや鉄球などの処理・描画を行うクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "ModeFadeComeBack.h"
#include "LoadingPlayer.h"
class ModeLoading : public ModeBase
{
public : 	
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
protected:
	LoadingPlayer* _chara;
};

