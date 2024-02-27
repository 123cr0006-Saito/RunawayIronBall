#pragma once
#include "BoardPolygon.h"
namespace PlaneEffect {
	class BoardPolygonDust : public PlaneEffectBase
	{
	public:
		// X�̒�������ɉ摜�̑傫������Y�𓱂��o���܂��B
		BoardPolygonDust(VECTOR pos, MATRIX matrix, int sizeX, int* handle, int handleMax, int speed); // �s��w���
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


