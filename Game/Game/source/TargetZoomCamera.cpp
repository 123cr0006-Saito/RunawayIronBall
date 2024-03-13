#include "TargetZoomCamera.h"

TargetZoomCamera::TargetZoomCamera() : CameraBase() {
	_IsEvent= false;
	_IsReturn = false;
	_moveTime = 0;
	_keepPos = GetCameraPosition();
	_targetPos = VGet(0, 0, 0);
	_keepPos = VGet(0, 0, 0);
};

TargetZoomCamera::TargetZoomCamera(VECTOR pos, int time, Camera* camera) :CameraBase() {
	_targetPos = pos;
	_moveTime = time;
	_keepPos = GetCameraPosition();
	_currentTime = GetNowCount();
	_IsEvent = true;
	_IsReturn = false;
};

TargetZoomCamera::~TargetZoomCamera() {

};

bool TargetZoomCamera::UpdateCameraToEvent() {
	int nowTime = GetNowCount() - _currentTime;
	VECTOR playerPos = _targetPos;

	//行列の掛け算
	MATRIX origin = MGetIdent();
	MATRIX MatrixX = MGetRotX(_cameraDirX);
	MATRIX MatrixY = MGetRotY(_cameraDirY);

	origin = MMult(origin, MatrixX);
	origin = MMult(origin, MatrixY);

	//注視点からの距離に行列を変換する
	VECTOR Vecter = VTransform(_pointDistance, origin);

	Vecter = VAdd(Vecter, _targetPos);

	//注視点の位置に移動
	VECTOR nowPos;
	if (nowTime > _moveTime) {
		nowTime = _moveTime;
		if (_IsReturn) {
			_IsReturn = _IsEvent = false;
		}
	}
	nowPos.x = Easing::Linear(nowTime, _keepPos.x, Vecter.x, _moveTime);
	nowPos.y = Easing::Linear(nowTime, _keepPos.y, Vecter.y, _moveTime);
	nowPos.z = Easing::Linear(nowTime, _keepPos.z, Vecter.z, _moveTime);

	SetCameraPositionAndTarget_UpVecY(nowPos, _targetPos);
	return true;
};

void TargetZoomCamera::SetEventCamera(VECTOR pos, int time) {
	_targetPos = pos;
	_moveTime = time;
	_keepPos = GetCameraPosition();
	_currentTime = GetNowCount();
	_IsEvent = true;
};

void TargetZoomCamera::SetReturnCamera(VECTOR pos) {
	if (_IsEvent) {
		_IsReturn = true;
		_currentTime = GetNowCount();
		_targetPos = pos;
		_keepPos = GetCameraPosition();
	}
}