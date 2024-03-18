#include "Camera.h"
Camera::Camera(VECTOR InitPos) : CameraBase() {

	SetCameraNearFar(20.0f, 30000.0f);
	
	_gazeShift = VGet(0, 120, 0);
	_reverseX = -1;
	_reverseY = 1;

	_cameraDistanceCount = 0;
	_startDistance = 0.0f;
	_endDistance = 0.0f;
	_zoomCount = 0;
	_IsZoom = false;

	float cameraChangeDistance[CAMERA_ZOOM_MAX] = { -400.0f, -600.0f, -800.0f };

	for (int i = 0; i < CAMERA_ZOOM_MAX; i++) {
		_cameraChangeDistance[i] = cameraChangeDistance[i];
	}

	_pointDistance.z =  _cameraChangeDistance[_cameraDistanceCount];

	//カメラの位置を計算
	MATRIX origin = MGetIdent();
	MATRIX MatrixX = MGetRotX(_cameraDirX);
	MATRIX MatrixY = MGetRotY(_cameraDirY);

	VECTOR playerPos = InitPos;

	//行列の掛け算
	origin = MMult(origin, MatrixX);
	origin = MMult(origin, MatrixY);

	//注視点からの距離に行列を変換する
	VECTOR Vecter = VTransform(_pointDistance, origin);

	//注視点の位置に移動
	VECTOR VecAdd = VAdd(Vecter, playerPos);
	float shiftY = 0;


	VECTOR target = VAdd(playerPos, VAdd(_gazeShift, VGet(0, shiftY, 0)));

	SetCameraPositionAndTarget_UpVecY(VecAdd, target);
};

Camera::~Camera() {
	_input = nullptr;
};

bool Camera::Process(VECTOR pos, int map) {
	float oldDirX = _cameraDirX;
	//入力から得た値で移動値を返す関数
	auto move_speed_process = [](float pos, float pos_max, float max_speed) {return pos * max_speed / pos_max; };

	//32768はshort型の最大値 移動速度の最大は0.02
	if (_input->GetRx() != 0) {
		_cameraDirY += move_speed_process(_input->GetRx(), 32768, 0.02) * _reverseY;
	}
	if (_input->GetRy() != 0) {
		_cameraDirX += move_speed_process(_input->GetRy(), 32768, 0.02) * _reverseX;
	}

	//RB入力でカメラの距離を変更
	if (_input->GetTrg(XINPUT_BUTTON_RIGHT_SHOULDER)) {
		SetCameraDistance();
	}

	//±で1周回ったら０度に変換
	if (abs(Math::RadToDeg(_cameraDirX)) > 360) {
		_cameraDirX = 0;
	}
	if (abs(Math::RadToDeg(_cameraDirY)) > 360) {
		_cameraDirY = 0;
	}

	// 回転値の上限
	if (_cameraDirX >= 1.39491415f) {
		_cameraDirX = 1.39491415f;
	}
	else if (_cameraDirX <= -1.39491415f) {
		_cameraDirX = -1.39491415f;
	}

	// ZoomFlagが立っていればzoomのprocessを回す
	ZoomProcess();

	//カメラの位置を計算
	MATRIX origin = MGetIdent();
	MATRIX MatrixX = MGetRotX(_cameraDirX);
	MATRIX MatrixY = MGetRotY(_cameraDirY);

	//引数としてプロセス内で得るのはめんどくさそいので
	//できればプレイヤークラスから引っ張ってきたいです
	VECTOR playerPos = pos;

	//行列の掛け算
	origin = MMult(origin, MatrixX);
	origin = MMult(origin, MatrixY);

	//注視点からの距離に行列を変換する
	VECTOR Vecter = VTransform(_pointDistance, origin);

	//注視点の位置に移動
	VECTOR VecAdd = VAdd(Vecter, playerPos);
	float shiftY = 0;


	VECTOR target = VAdd(playerPos, VAdd(_gazeShift, VGet(0, shiftY, 0)));


	MV1_COLL_RESULT_POLY_DIM HitPolyDim;
	float cameraR = 75.0f;
	HitPolyDim = MV1CollCheck_Capsule(map, -1, target, VecAdd, cameraR);
	if (HitPolyDim.HitNum >= 1) {
		VECTOR dirVec = VSub(target, VecAdd);
		//ポリゴンの位置に設定
		VecAdd.y = VScale(VAdd(VAdd(HitPolyDim.Dim[0].Position[0], HitPolyDim.Dim[0].Position[1]), HitPolyDim.Dim[0].Position[1]), 0.3333333).y;
		//カプセルの半径である５０分y軸方向に移動
		VecAdd.y += cameraR;

		_cameraDirX = oldDirX;
	}
	MV1CollResultPolyDimTerminate(HitPolyDim);

	//カメラのセット
	SetCameraPositionAndTarget_UpVecY(VecAdd, target);
	return true;
};

void Camera::SetCameraDistance() {
	_cameraDistanceCount++;
	_cameraDistanceCount = (_cameraDistanceCount + CAMERA_ZOOM_MAX) % CAMERA_ZOOM_MAX;
	_startDistance = _pointDistance.z;
	_endDistance = _cameraChangeDistance[_cameraDistanceCount];
	_zoomCount = 0;
	_IsZoom = true;
	_currentTime = GetNowCount();
};

bool Camera::ZoomProcess() {
	if (_IsZoom) {
		float moveTime = 5.0f ;// 5フレームで移動
		_zoomCount++;
		// 移動
		_pointDistance.z = Easing::InQuad(_zoomCount,_startDistance,_endDistance, moveTime);
		// 終了
		if (_zoomCount >= 5) {
			_IsZoom = false;
		}
	}
	return true;
};