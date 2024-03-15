#include "AppFrame.h"
#include "ModeClear.h"
bool ModeClear::Initialize(){
	if (!base::Initialize()) { return false; }
	input = XInput::GetInstance();
	_model = ResourceServer::Load("Player", "res/Character/cg_player_girl/cg_player_girl_TEST_Ver.2.mv1");
	_frameCount = 0;
	_attachAnim = MV1AttachAnim(_model, MV1GetAnimIndex(_model, "MO_PL_Win_Pose"), -1, false);
	_maxCount = MV1GetAnimTotalTime(_model, _attachAnim);
	_backGround = ResourceServer::Load("skysphere","res/Scenario/BackGround/skysphere.png");
	SetCameraNearFar(20.0f, 30000.0f);
	MV1SetPosition(_model, VGet(0, 0, 0));
	return true;
};

bool ModeClear::Terminate(){
	base::Terminate();
	input = nullptr;
	return true;
};

bool ModeClear::Process(){
	base::Process();
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	if (_frameCount >= _maxCount){
	//	ModeServer::GetInstance()->Del(this);
	}
	MV1SetAttachAnimTime(_model, _attachAnim, _frameCount);

	VECTOR pos = MV1GetFramePosition(_model, MV1SearchFrame(_model, "camera1"));
	VECTOR target = MV1GetFramePosition(_model, MV1SearchFrame(_model, "camera1_aim"));

	SetCameraPositionAndTarget_UpVecY(pos,target);
	_frameCount++;
	return true;
};

bool ModeClear::Render() {
	base::Render();
	DrawGraph(0, 0, _backGround, FALSE);
	MV1DrawModel(_model);
	return true;
};