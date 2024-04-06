//----------------------------------------------------------------------
// @filename TargetZoomCamera.cpp
// ＠date: 2024/03/09
// ＠author: saito ko
// @explanation
// 今のカメラの位置を指定の位置に移動する処理を行うクラス
//----------------------------------------------------------------------
#include "TargetZoomCamera.h"

TargetZoomCamera::TargetZoomCamera(VECTOR target, VECTOR nextPos, VECTOR keepDir, VECTOR nextDir, int time) :CameraBase() {
	_targetPos = target;
	_nextPos = nextPos;
	_keepPos = GetCameraPosition();
	_nextDir = nextDir;
	_keepDir = keepDir;
	_moveTime = time;
	_currentTime = GetNowCount();

};

TargetZoomCamera::~TargetZoomCamera() {

};

bool TargetZoomCamera::Process() {
	int nowTime = GetNowCount() - _currentTime;

	//注視点の位置に移動
	VECTOR nowPos;
	VECTOR nowTarget;
	if (nowTime > _moveTime) {
		nowTime = _moveTime;
	}
	float deray = 1.0f;
	float targetEasingTime = nowTime;
	if (targetEasingTime > nowTime / deray) {
		targetEasingTime = nowTime / deray;
	}

	nowTarget.x = Easing::Linear(targetEasingTime, _keepDir.x, _nextDir.x, _moveTime / deray);
	nowTarget.y = Easing::Linear(targetEasingTime, _keepDir.y, _nextDir.y, _moveTime / deray);
	nowTarget.z = Easing::Linear(targetEasingTime, _keepDir.z, _nextDir.z, _moveTime / deray);

	nowPos.x = Easing::Linear(nowTime, _keepPos.x, _nextPos.x, _moveTime);
	nowPos.y = Easing::Linear(nowTime, _keepPos.y, _nextPos.y, _moveTime);
	nowPos.z = Easing::Linear(nowTime, _keepPos.z, _nextPos.z, _moveTime);

	nowTarget = VAdd(nowPos, nowTarget);

	SetCameraPositionAndTarget_UpVecY(nowPos, _targetPos);
	return true;
};