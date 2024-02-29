#include "BoardPolygonDust.h"


	BoardPolygonDust::BoardPolygonDust(VECTOR pos, int sizeX, int* handle, int handleMax, int speed,int animspeed) 
	: PlaneEffectBase(pos,  sizeX, handle, handleMax, animspeed) {
		_standardPos = pos;
		_speed = speed;
		VECTOR oPos = VGet(0, 0, 0);
		_vLength = oPos;
		for (int i = 0; i < _dustMax; i++){
			_dustPos[i] = oPos;
		}
	};

	BoardPolygonDust::~BoardPolygonDust() {
		PlaneEffectBase::~PlaneEffectBase();
	};

	bool BoardPolygonDust::Process() {
		PlaneEffectBase::Process();
		MATRIX billboardMatrix = GetCameraBillboardMatrix();
		_vLength.y += static_cast<float>(_speed) / 2;
		_vLength.z += static_cast<float>(_speed);
		MATRIX matrix = MGetTranslate(_vLength);
		VECTOR originPos = VGet(1, 1, 1);
		for (int i = 0; i < _dustMax; i++) {
			float rotation = 360.0f / _dustMax * i * DX_PI / 180.0f;
			matrix = MMult(matrix, MGetRotY(rotation));
			VECTOR vec = VTransform(originPos, matrix);
			_dustPos[i] = VAdd(_standardPos, vec);
		}
		return true;
	};

	bool BoardPolygonDust::Render() {
		for (int i = 0; i < _dustMax; i++) {
			DrawBillboard3D(_dustPos[i],0.5f,0.3f,_sizeX,0.0f, _handle[_animCount],true);
		}
		return true;
	};
