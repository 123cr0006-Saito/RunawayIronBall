#pragma once
#include "PlaneEffectBase.h"
class BoardPolygon : public PlaneEffectBase
{
public:
	BoardPolygon(VECTOR pos, VECTOR dir, int sizeX, int* handle, int handleMax, int speed);//�����x�N�g���w���
	BoardPolygon(VECTOR pos, MATRIX matrix, int sizeX, int* handle, int handleMax, int speed);//�s��w���
	~BoardPolygon();
	bool Process()override;
	bool Render()override;
	bool GetFlag() { return _IsPlay; }
protected:
	VERTEX3D polygon[4];
};
