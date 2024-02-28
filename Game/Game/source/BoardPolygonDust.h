#pragma once
#include "BoardPolygon.h"
namespace PlaneEffect {
	class BoardPolygonDust : public PlaneEffectBase
	{
	public:
		// Xの長さを基準に画像の大きさからYを導き出します。
		BoardPolygonDust(VECTOR pos,int sizeX, int* handle, int handleMax,int animspeed ,int speed = 10);
		~BoardPolygonDust();
		bool Update()override;
		bool Render()override;
	protected:
		static const int _dustMax = 10;
		int _speed;
		VECTOR _dustPos[_dustMax];
		VECTOR _standardPos;
		VECTOR _vLength;
	};
};


