#include "AppFrame.h"
#include "ModeClear.h"
#include "ModeFadeComeBack.h"

ModeClear::ModeClear() {
	_modeGame = nullptr;
	input = nullptr;
	_model = 0;
	_attachAnim = 0;
	_frameCount = 0;
	_maxCount = 0;
	_backGround = 0;
	_alphaValue = 0;
	_currentTime = 0;
	_valuationHandle = 0;
};

ModeClear::ModeClear(ModeGame* mode){
	_modeGame = mode;
	input = nullptr;
	 _model = 0;
	 _attachAnim = 0;
	 _frameCount = 0;
	 _maxCount = 0;
	 _backGround = 0;
	 _alphaValue = 0;
	 _currentTime = 0;
	 _valuationHandle = 0;
};

bool ModeClear::Initialize(){
	if (!base::Initialize()) { return false; }
	input = XInput::GetInstance();
	_model = ResourceServer::Load("Player", "res/Character/cg_player_girl/cg_player_girl_TEST_Ver.2.mv1");
	_frameCount = 0;
	int animNum = MV1GetAnimIndex(_model, "MO_PL_Win_Pose");
	_attachAnim = MV1AttachAnim(_model,animNum , -1, false);
	_maxCount = MV1GetAnimTotalTime(_model, animNum);
	_backGround = ResourceServer::Load("skysphere","res/Scenario/BackGround/skysphere.png");
	SetCameraNearFar(20.0f, 30000.0f);
	MV1SetPosition(_model, VGet(0, 0, 0));

	Valuation();

	return true;
};

bool ModeClear::Terminate(){
	base::Terminate();
	input = nullptr;
	_modeGame = nullptr;
	return true;
};

void ModeClear::AnimProcess(){
	MV1SetAttachAnimTime(_model, _attachAnim, _frameCount);

	VECTOR pos = MV1GetFramePosition(_model, MV1SearchFrame(_model, "camera1"));
	VECTOR target = MV1GetFramePosition(_model, MV1SearchFrame(_model, "camera1_aim"));

	SetCameraPositionAndTarget_UpVecY(pos, target);

	if (_frameCount < _maxCount) {
		//アニメーションの再生時間を設定
	    _frameCount += 1.0f;
		// アニメーション終了時現在の時間を取得
		if(_frameCount == _maxCount){
			_currentTime = GetNowCount();
		}
	}
};

void ModeClear::Valuation(){

};

void ModeClear::ValuationProcess(){
	if (_frameCount >= _maxCount) {
		int endTime = 2 * 1000;
		int nowTime = GetNowCount() - _currentTime;
		// アルファ値の変化
		if(nowTime > endTime){
			nowTime = endTime;
		}
		_alphaValue = Easing::Linear(nowTime,0, 255, endTime);

	   if (_alphaValue >= 255 && input->GetTrg(XINPUT_BUTTON_A)) {
		   ModeServer::GetInstance()->Add(NEW ModeFadeComeBack(2000,this), 100, "Fade");
		   if(_modeGame != nullptr){
	   	   _modeGame->NewStage();
		   }
	   }
	}
};

bool ModeClear::Process(){
	base::Process();
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	ModeServer::GetInstance()->SkipRenderUnderLayer();

	AnimProcess();
	ValuationProcess();

	return true;
};

bool ModeClear::Render() {
	base::Render();
	DrawGraph(0, 0, _backGround, FALSE);
	MV1DrawModel(_model);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alphaValue);
	//DrawGraph();
	DrawBox(1820, 980, 1920, 1080, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, _alphaValue);
	return true;
};