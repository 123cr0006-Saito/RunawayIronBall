//----------------------------------------------------------------------
// @filename CrystarRoof.h
// @date: 2024/02/24
// @author: saito ko
// @explanation
// クライスターの屋根部分を描画するためのクラス
//----------------------------------------------------------------------
#pragma once
#include "DxLib.h"
#include "appframe.h"
class CrystarRoof
{
public:
	CrystarRoof(int model, int parent,std::string frameName);
	~CrystarRoof();
	bool Update();
	bool Render();
protected:
	int _model;//モデルハンドル
	int _frame;//フレームの番号
	int _parent;//親のモデルハンドル
	std::string _joint;//親のモデルのどのジョイントにくっつけるか
};

