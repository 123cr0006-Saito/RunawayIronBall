#include "RotationCamera.h"
RotationCamera::RotationCamera(int time) : CameraBase() {
	_pointDistance.z = -2000;
	_endTime = time;
};

RotationCamera::~RotationCamera() {

};

bool RotationCamera::Process() {

	float _cameraDirYAdd = 0.04f;
	_cameraDirY += _cameraDirYAdd;

	//カメラの位置を計算
	MATRIX origin = MGetIdent();
	MATRIX MatrixX = MGetRotX(_cameraDirX);
	MATRIX MatrixY = MGetRotY(_cameraDirY);

	//引数としてプロセス内で得るのはめんどくさそいので
	//できればプレイヤークラスから引っ張ってきたいです
	VECTOR target = VGet(0,500,0);

	//行列の掛け算
	origin = MMult(origin, MatrixX);
	origin = MMult(origin, MatrixY);

	//注視点からの距離に行列を変換する
	VECTOR Vecter = VTransform(_pointDistance, origin);

	//注視点の位置に移動
	VECTOR VecAdd = VAdd(Vecter, target);

	//カメラのセット
	SetCameraPositionAndTarget_UpVecY(VecAdd, target);

	if (GetNowCount() - _currentTime >= _endTime) {
	
	}
};