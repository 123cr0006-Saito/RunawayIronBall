//----------------------------------------------------------------------
// @filename UITimeLimit.h
// @date: 2024/03/11
// @author: saito ko
// @explanation
// 残り時間を表示するUIクラス
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
	TimeLimit* _timeLimit;// 残り時間クラス
	VECTOR _numPos;// 桁数の位置
	int _colonHandle;// コロンの画像
	int _frameHandle;// フレームの画像
};

