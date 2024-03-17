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
	_alphaValue = 0;
	_currentTime = 0;
	_valuation = 0;
	_valuationTime = 0
};

ModeClear::ModeClear(ModeGame* mode){
	_modeGame = mode;
	input = nullptr;
	 _model = 0;
	 _attachAnim = 0;
	 _frameCount = 0;
	 _maxCount = 0;
	 _alphaValue = 0;
	 _currentTime = 0;
	 _valuation = 0;
	 _valuationTime = 0;
};

bool ModeClear::Initialize(){
	if (!base::Initialize()) { return false; }
	input = XInput::GetInstance();
	_model = ResourceServer::Load("Player", "res/Character/cg_player_girl/Cg_Player_Girl.mv1");
	_frameCount = 0;
	int animNum = MV1GetAnimIndex(_model, "MO_PL_Win_Pose");
	_attachAnim = MV1AttachAnim(_model,animNum , -1, false);
	_maxCount = MV1GetAnimTotalTime(_model, animNum);
	_handle["BackGround"] = ResourceServer::Load("skysphere", "res/ModeScenario/BackGround/skysphere.png");
	_handle["Time"] = ResourceServer::Load("Time", "res/ModeResult/Time/UI_Valuation_Time_Logo.png");
	_handle["Colon"] = ResourceServer::Load("Colon", "res/ModeResult/Time/UI_Valuation_Time_Colon.png");
	ResourceServer::LoadMultGraph("Valuation","res/ModeResult/Rank/UI_Valuation",".png",4,_valuationHandle);
	ResourceServer::LoadMultGraph("C_Time","res/ModeResult/Time/UI_Valuation_Time",".png",10,_timeHandle);
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
		//�A�j���[�V�����̍Đ����Ԃ�ݒ�
	    _frameCount += 1.0f;
		// �A�j���[�V�����I�������݂̎��Ԃ��擾
		if(_frameCount == _maxCount){
			_currentTime = GetNowCount();
		}
	}
};

void ModeClear::Valuation(){

	if(TimeLimit::GetInstance() != nullptr){
		TimeLimit* time = TimeLimit::GetInstance();
		_valuationTime =time->GetElapsedTime();
		int startTime = time->GetStartTime ();
		int valuationCount = 3; // 0 s 1 a 2 b 3 c
		float valuationPercentage[3] = {7.5f,5.0f,3.0f};
		for(int i = 0; i < 3; i++){
			int aaa = startTime / 10 * valuationPercentage[i];
		   if(_valuationTime <=  aaa)valuationCount--;
		}
		_valuation = valuationCount;
	}
};

void ModeClear::ValuationProcess(){
	if (_frameCount >= _maxCount) {
		int endTime = 2 * 1000;
		int nowTime = GetNowCount() - _currentTime;
		// �A���t�@�l�̕ω�
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
	// �w�i�̕`��
	DrawGraph(0, 0, _handle["BackGround"], FALSE);
	// ���f���̕`��
	MV1DrawModel(_model);
	// ���ߐF
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alphaValue);
	int handleX,handleY,screenX,screenY,screenDepth;
	GetGraphSize(_valuationHandle[_valuation],&handleX,&handleY);
	GetScreenState(&screenX,&screenY,&screenDepth);
	// �]���̕\��
	DrawGraph(screenX - handleX - 50, screenY - handleY - 50, _valuationHandle[_valuation],true);

	// ���Ԃ̕\��
	int time = _valuationTime;
	int loopCount = 0;
	int x = 1600, y = 400;
	while (1) {
		
		if (loopCount == 2) {
			// �R������`��
			GetGraphSize(_handle["Colon"], &handleX, &handleY);
			DrawGraph(x + handleX -40, y + 10, _handle["Colon"], true);
			x -= handleX + 20;// �����̊Ԋu
		}

		// �b����`��
		int num = time % 10;
		DrawGraph(x, y, _timeHandle[num], true);
		time /= 10;

		GetGraphSize(_timeHandle[num], &handleX, &handleY);

		x -= handleX + 10;// �����̊Ԋu
		loopCount++;// �����ڂ�������

		if (time == 0) {
			break;
		}

	}
	// time�n���h���̕`��
	GetGraphSize(_handle["Time"], &handleX, &handleY);
	DrawGraph(x - handleX/2+50,y - handleY/2-80,_handle["Time"],true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, _alphaValue);
	return true;
};