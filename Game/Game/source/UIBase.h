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
	UIBase(VECTOR pos, int size, int* handle);
	virtual ~UIBase();
	virtual bool Process();
	virtual bool Draw();

protected:
	int* _handle;// 画像ハンドル
	int _handleNum;// 画像ハンドルの数
	VECTOR _pos;// 位置
	float _cx, _cy;// 中心座標
};
