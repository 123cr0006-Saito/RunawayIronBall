#include "ModeRotationCamera.h"
#include "TimeLimit.h"

ModeRotationCamera::ModeRotationCamera(int stageNum) {
	_camera = nullptr;
	_currentTime = 0;
	_handle = 0;
	_stageNum = stageNum;
};

bool ModeRotationCamera::Initialize(){
	float distance = 6000;
	VECTOR targetPos = VGet(0, 2000, 0);
	_camera = NEW RotationCamera(distance,targetPos);
	_input = XInput::GetInstance();
	_currentTime = GetNowCount();
	int Num = _stageNum;
	_handle = ResourceServer::LoadGraph("StageHandle" + std::to_string(Num),"res/StageName/Stage" + std::to_string(Num) + ".png");
	return true;
};

bool ModeRotationCamera::Terminate(){
	delete _camera; _camera = nullptr;
	if(TimeLimit::GetInstance() != nullptr){
	    TimeLimit::GetInstance()->Restart();
	}
	return true;
};

bool ModeRotationCamera::Process() {
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();

	_camera->Process();

	int endTime = 10 * 1000;
	int nowTime = GetNowCount() - _currentTime;
	if (nowTime > endTime) {
		bool SearchFade = ModeServer::GetInstance()->Search("Fade");
		if (!SearchFade || _input->GetTrg(XINPUT_BUTTON_START)) {
			ModeServer::GetInstance()->Add(NEW ModeFadeComeBack(3000, this), 100, "Fade");
		}
	}
	return true;
};

bool ModeRotationCamera::Render() {
	int handleX, handleY, windowX, windowY, windowDepth;
	GetGraphSize(_handle, &handleX, &handleY);
	GetScreenState(&windowX, &windowY, &windowDepth);
	DrawGraph((windowX - handleX)/2,100 , _handle, true);
	return true;
};