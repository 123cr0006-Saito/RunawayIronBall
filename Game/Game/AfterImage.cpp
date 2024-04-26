//----------------------------------------------------------------------
// @filename AfterImage.cpp
// ＠date: 2024/04/26
// ＠author: Morozumi Hiroya
// @explanation
// キャラクターの残像表現を行うクラス
// 半透明にしたモデルを複数体表示することで残像を表現する
//----------------------------------------------------------------------
#include "AfterImage.h"

AfterImage::AfterImage()
{
}

AfterImage::~AfterImage()
{
	for(auto itr = _modelInfo.begin(); itr != _modelInfo.end(); ++itr)
	{
		delete (*itr);
		(*itr) = nullptr;
	}
	_modelInfo.clear();
}


