//----------------------------------------------------------------------
// @filename Light.h
// @date: 2024/03/01
// @author: saito ko
// @explanation
// ファイルからデータを読み込みライトを設定するクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class Light
{
public :
	Light(std::string fileName);
	~Light();
	bool LoadLightColor(std::string fileName);
protected:
	int _lightHandle;//ライトハンドル
};

