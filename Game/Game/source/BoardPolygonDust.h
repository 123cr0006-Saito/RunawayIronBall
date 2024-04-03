//----------------------------------------------------------------------
// @filename BoardPolygonDust.h
// ＠date: 2024/02/28
// ＠author: saito ko
// @explanation
// 砂煙の板ポリを生成するのクラス
//----------------------------------------------------------------------
#pragma once
#include "BoardPolygon.h"
#include "PlaneEffectBase.h"

class BoardPolygonDust : public PlaneEffectBase
{
public:
	// Xの長さを基準に画像の大きさからYを導き出します。
	BoardPolygonDust(VECTOR pos,int sizeX, int* handle, int handleMax,int animspeed ,int speed = 10);
	~BoardPolygonDust();
	bool Process()override;
	bool Render()override;
protected:
	static const int _dustMax = 10;
	int _speed;
	VECTOR _dustPos[_dustMax];
	VECTOR _standardPos;
	VECTOR _vLength;
};



