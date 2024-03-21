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

	//’Ž‹“_‚ÌˆÊ’u‚ÉˆÚ“®
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