//----------------------------------------------------------------------
// @filename UITimeLimit.h
// ��date: 2024/03/11
// ��author: saito ko
// @explanation
// �c�莞�Ԃ�\������UI�N���X
//----------------------------------------------------------------------
#pragma once
#include "UIBase.h"
#include "TimeLimit.h"
class UITimeLimit : public UIBase
{
public : 
	UITimeLimit(VECTOR pos,VECTOR numPos, int size, int* numhandle ,int colonHandle,int _frameHandle);
	UITimeLimit(VECTOR pos);
	~UITimeLimit();
	bool Process();
	bool Draw();
protected:
	TimeLimit* _timeLimit;
	VECTOR _numPos;
	int _colonHandle;
	int _frameHandle;
};

