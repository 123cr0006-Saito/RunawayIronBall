#include "BoardPolygonDust.h"
namespace PlaneEffect {
	BoardPolygonDust::BoardPolygonDust(VECTOR pos, MATRIX matrix, int sizeX, int* handle, int handleMax, int speed) 
	: PlaneEffectBase(pos,  sizeX, handle, handleMax, speed) {
		_standardPos = pos;
	};
	BoardPolygonDust::~BoardPolygonDust() {
		delete[] _dustPos;
	};
	bool BoardPolygonDust::Update() {
		PlaneEffectBase::Update();
		MATRIX billboardMatrix = GetCameraBillboardMatrix();
		MATRIX matrix = MGetTranslate(VGet(0, 0, length));
		for (int i = 0; i < _dustMax; i++) {
			float rotation = 360.0f / _dustMax * i;
			matrix = MMult(matrix, MGetRotY(rotation));
			matrix = MMult(matrix, billboardMatrix);
			_dustPos[i] = VTransform(_standardPos, matrix);
		}
		return true;
	};
	bool BoardPolygonDust::Render() {
		for (int i = 0; i < _dustMax; i++) {
			DrawBillboard3D(_dustPos[i],0.5f,0.5f,_sizeX,0.0f, _handle[_animCount],true);
		}
		return true;
	};
}