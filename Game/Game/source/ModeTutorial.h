//----------------------------------------------------------------------
// @filename ModeTutorial.h
// @date: 2024/03/17
// @author: saito ko
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
	XInput* _input;// 入力クラス
	int* _handle;// 画像ハンドルの配列
	int _pageSize;// 画像の枚数
	int _selectItem;// 描画中の項目番号
};