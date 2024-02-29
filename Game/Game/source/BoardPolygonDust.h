#pragma once
#include "BoardPolygon.h"
#include "PlaneEffectBase.h"

	class BoardPolygonDust : public PlaneEffectBase
	{
	public:
		// X�̒�������ɉ摜�̑傫������Y�𓱂��o���܂��B
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



