//----------------------------------------------------------------------
// @filename ModeClear.cpp
// ＠date: 2024/03/16
// ＠author: saito ko
// @explanation
// クリア時にリザルトと次のステージへの遷移を管理するクラス
//----------------------------------------------------------------------
#include "AppFrame.h"
#include "ModeClear.h"
#include "ModeScenario.h"
#include "ModeFadeComeBack.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param elapsedTime 経過時間
// @param startTime 開始時間
//----------------------------------------------------------------------
ModeClear::ModeClear(int elapsedTime, int startTime) {
	_input = nullptr;
	_model = 0;
	_attachAnim = 0;
	_frameCount = 0;
	_maxCount = 0;
	_alphaValue = 0;
	_currentTime = 0;
	_valuation = 0;
	_valuationTime = 0;
	_nowValuationTime = 0;
	_valuationSize = 0.0f;
	_IsStaging = false;
	_IsNextStage = false;

	Valuation(elapsedTime,startTime);
};

bool ModeClear::Initialize(){
	if (!base::Initialize()) { return false; }
	_input = XInput::GetInstance();
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
	global._soundServer->DirectPlay("Result");

	return true;
};

bool ModeClear::Terminate(){
	base::Terminate();
	_input = nullptr;
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
			_IsStaging = true;
		}
	}
};

void ModeClear::Valuation(int elapsedTime, int startTime){

		_valuationTime = elapsedTime;
		int gameStartTime = startTime;
		int valuationCount = 3; // 0 s 1 a 2 b 3 c
		float valuationPercentage[3] = {10.0f,7.5f,5.0f};
		for(int i = 0; i < 3; i++){
			int Parcentage = gameStartTime / 10 * valuationPercentage[i];
		   if(_valuationTime <= Parcentage)valuationCount--;
		}
		_valuation = valuationCount;
	
};

void ModeClear::ValuationProcess(){
	if (!_IsStaging)return;

		int stagingTime = GetNowCount() - _currentTime;

		auto Easing = [](float time, float start, float end, float duration) {
			float temp = 0;
			if (time > 0) {
				if (time > duration) {
					time = duration;
				}
				 temp = Easing::Linear(time, start, end, duration);
			}
			return temp;
		};

		if(stagingTime > 500 * _chain.size() && _chain.size() < 4){
			VECTOR pos[4] = {VGet(900,100,0),VGet(200,500,0),VGet(1800,600,0), VGet(1700,800,0)};
			float angle[4] = {-5,50,100,-15};
			_chain.push_back(NEW AnimationChain(pos[_chain.size()], angle[_chain.size()]));
		}

		// Timeアルファ値処理
		int timeAlphaEndTime = 1 * 1000;
		int timeAlphaTime = stagingTime;
		 _alphaValue = Easing(timeAlphaTime, 0, 255, timeAlphaEndTime);
        
		stagingTime -= timeAlphaEndTime;

		// 評価の時間変化
		int valuationEndTime = 2 * 1000;
		int valuationTime = stagingTime;
		 _nowValuationTime = Easing(valuationTime,0, _valuationTime, valuationEndTime);
        
		stagingTime -= valuationEndTime;

		// 0.5秒後に評価のサイズを変化
		stagingTime -= 500;

		// 評価の変化
		int valuationSizeEndTime = 0.5 * 1000;
		int valuationSizeTime = stagingTime;
		_valuationSize = Easing(valuationSizeTime, 1.50f, 1.0f, valuationSizeEndTime);

		if (valuationSizeTime > valuationSizeEndTime) {
			_IsNextStage = true;
		}

};

bool ModeClear::Process(){
	base::Process();
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	ModeServer::GetInstance()->SkipRenderUnderLayer();

	AnimProcess();
	ValuationProcess();

	for (auto chain : _chain) {
		chain->Process();
	}

	if (_IsNextStage && _input->GetTrg(XINPUT_BUTTON_A) && !ModeServer::GetInstance()->Search("Fade")) {
		ModeServer::GetInstance()->Add(NEW ModeFadeComeBack(1000, this), 100, "Fade");
		if (global.GetStageNum() < 4) {
			ModeServer::GetInstance()->Add(NEW ModeGame(), 1, "Game");
		}
		else {
			ModeServer::GetInstance()->Add(NEW ModeScenario("Data/ScenarioData/Scenario02.csv", 2), 0, "Scenario");
			ModeServer::GetInstance()->Add(NEW ModeFadeComeBack(1000, this,"Scenario",50), 100, "Fade");
		}
	}

	return true;
};

bool ModeClear::Render() {
	base::Render();
	// 背景の描画
	DrawGraph(0, 0, _handle["BackGround"], FALSE);
	// モデルの描画
	MV1DrawModel(_model);

	int handleX, handleY, screenX, screenY, screenDepth;
	GetScreenState(&screenX, &screenY, &screenDepth);

	for (auto chain : _chain) {
		chain->Draw();
	}

	// 透過色
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alphaValue);

	// timeハンドルの描画
	GetGraphSize(_handle["Time"], &handleX, &handleY);
	DrawGraph(1350, 100, _handle["Time"], true);

	// 時間の表示
	int time = static_cast<int>(_nowValuationTime);
	int loopCount = 0;
	int x = 1800, y = 150;
	while (1) {

		if (loopCount == 2) {
			// コロンを描画
			GetGraphSize(_handle["Colon"], &handleX, &handleY);
			DrawRotaGraph(x + handleX - 40 + handleX/2, y + 10 + handleY / 2.0f ,1.0f,-30*DX_PI/180.0f ,_handle["Colon"], true);
			x -= handleX + 20;// 数字の間隔
			y += handleY / 2.0f;// 数字の間隔
		}

		// 秒数を描画
		int num = time % 10;
		GetGraphSize(_timeHandle[num], &handleX, &handleY);
		DrawRotaGraph(x+ handleX/2, y + handleY / 2,1.0f,-30*DX_PI/180.0f, _timeHandle[num], true);
		time /= 10;

		x -= handleX + 10;// 数字の間隔
		y += handleY/2.0f;// 数字の間隔
		loopCount++;// 何桁目か数える

		if (loopCount == 4) {
			break;
		}
	}
	// 透過色終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, _alphaValue);

	// 評価の表示
	GetGraphSize(_valuationHandle[_valuation], &handleX, &handleY);
	DrawRotaGraph(1100 + handleX/2, 450 + handleY/2, _valuationSize, -5 * DX_PI / 180, _valuationHandle[_valuation], true);

	return true;
};