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
		bool a = ModeServer::GetInstance()->Search("Fade");
		if (!a) {
			ModeServer::GetInstance()->Add(NEW ModeFadeComeBack(4000, this), 100, "Fade");
		}
	}
	return true;
};