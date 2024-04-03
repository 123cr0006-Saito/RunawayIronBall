//----------------------------------------------------------------------
// @filename ModeLoading.h
// ��date: 2024/03/06
// ��author: saito ko
// @explanation
// loading���̃L�����N�^�[��S���Ȃǂ̏����E�`����s���N���X
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

