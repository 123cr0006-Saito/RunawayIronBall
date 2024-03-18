#pragma once
#include "appframe.h"
#include "dxlib.h"
#include <vector>
class ModeInstructions : public ModeBase
{
	typedef ModeBase base;
public :
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

protected:
	XInput* _input;
	//項目の最大数
	static const int LIST_SIZE_MAX = 5;
	static const int _listViewMax = 5;//画面に表示させる項目の最大サイズ

	int _listChoice;//項目の中にあるカーソルの位置
	int _listViewNum;//画面に表示する項目の場所
	

	int _frameHandle;
	int _itemHandle[5];//選択された項目の画像
	int _imageHandle[5];//実際の画面の画像
};

