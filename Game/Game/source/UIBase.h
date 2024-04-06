//----------------------------------------------------------------------
// @filename UIBase.h
// ＠date: 2023/12/19
// ＠author: saito ko
// @explanation
// UIの基底クラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "dxlib.h"
#include <string>
//画面に表示するUIの基盤となるクラス
class UIBase
{
public:
	UIBase(VECTOR pos);
	UIBase(VECTOR pos, std::string handleName);
	UIBase(VECTOR pos, std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf);
	UIBase(VECTOR pos, int size, int* handle);
	virtual ~UIBase();

	virtual bool Process();
	virtual bool Draw();

protected:
	int* _handle;
	int _handleNum;
	VECTOR _pos;
	float _cx, _cy;
};
