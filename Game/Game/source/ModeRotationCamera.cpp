#include "ModeRotationCamera.h"
bool ModeRotationCamera::Initialize(){
	float distance = 6000;
	VECTOR targetPos = VGet(0, 2000, 0);
	_camera = NEW RotationCamera(distance,targetPos);
	_currentTime = GetNowCount();
	return true;
};

bool ModeRotationCamera::Terminate(){
	delete _camera; _camera = nullptr;
	return true;
};

bool ModeRotationCamera::Process() {
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();

	_camera->Process();

	int endTime = 10 * 1000;
	int nowTime = GetNowCount() - _currentTime;
	if (nowTime > endTime) {
		ModeServer::GetInstance()->Add(NEW ModeFade(4000, true),100,"Fade");
		ModeServer::GetInstance()->Del(this);
	}
	return true;
};