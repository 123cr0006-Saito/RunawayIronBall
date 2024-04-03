//----------------------------------------------------------------------
// @filename EffectBase.cpp
// ��date: 2024/01/12
// ��author: saito ko
// @explanation
// �G�t�F�N�g���Đ����邽�߂̊��N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class EffectBase
{
public : 
	EffectBase();
	virtual ~EffectBase();
	virtual bool Process();
	virtual bool Render();
	bool GetFlag() { return _IsPlay; }
protected : 		
	bool _IsPlay;
};

