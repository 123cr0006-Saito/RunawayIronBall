#pragma once
#include "appframe.h"
#include "dxlib.h"
#include <vector>
#define NO_IMAGE 
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
	static const int LIST_SIZE_MAX = 20;
	static const int _listViewMax = 8;//画面に表示させる項目の最大サイズ

	int _listChoice;//項目の中にあるカーソルの位置
	int _listViewNum;//画面に表示する項目の場所
	
	struct SUMMARIZE{//表示させたい画像をリンクさせるようにするための構造体
		int item;//選択された項目の画像
		int image;//実際の画面の画像
		int explanation;//説明文の画像
	};

	SUMMARIZE _handle[LIST_SIZE_MAX];//操作説明の項目の個数
};

