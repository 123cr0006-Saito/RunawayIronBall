#pragma once
#include "PlaneEffectBase.h"
class BoardPolygon : public PlaneEffectBase
{
public:
	BoardPolygon(VECTOR pos, VECTOR dir, int sizeX, int* handle, int handleMax, int speed);//方向ベクトル指定版
	BoardPolygon(VECTOR pos, MATRIX matrix, int sizeX, int* handle, int handleMax, int speed);//行列指定版
	~BoardPolygon();
	bool Process()override;
	bool Render()override;
	bool GetFlag() { return _IsPlay; }
protected:
	VERTEX3D polygon[4];
};
