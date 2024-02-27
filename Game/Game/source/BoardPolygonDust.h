#pragma once
#include "BoardPolygon.h"
namespace PlaneEffect {
	class BoardPolygonDust : public PlaneEffectBase
	{
	public:
		// Xの長さを基準に画像の大きさからYを導き出します。
		BoardPolygonDust(VECTOR pos, MATRIX matrix, int sizeX, int* handle, int handleMax, int speed); // 行列指定版
		~BoardPolygonDust();
		bool Update()override;
		bool Render()override;
	protected:
		static const int _dustMax = 10;
		float length;
		VECTOR _dustPos[_dustMax];
		VECTOR _standardPos;
	};
};


