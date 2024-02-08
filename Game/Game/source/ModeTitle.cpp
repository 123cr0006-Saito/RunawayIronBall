
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeTitle.h"
#include "ModeGame.h"
#include "ModeTest.h"
#include "ModePause.h"
#include "math.h"


bool ModeTitle::Initialize() {
	if (!base::Initialize()) { return false; }

	_input = XInput::GetInstance();
	_modeCount = 0;
	_titleLogo = ResourceServer::LoadGraph(_T("res/TemporaryMaterials/Title/title.png"));
	_comandHandlle[0] = ResourceServer::LoadGraph(_T("res/TemporaryMaterials/Title/UI_starta_select.png"));
	_comandHandlle[1] = ResourceServer::LoadGraph(_T("res/TemporaryMaterials/Title/UI_option_select.png"));
	_comandHandlle[2] = ResourceServer::LoadGraph(_T("res/TemporaryMaterials/Title/UI_quit.png"));
	_comandHandlle[3] = ResourceServer::LoadGraph(_T("res/TemporaryMaterials/Title/UI_start.png"));
	_comandHandlle[4] = ResourceServer::LoadGraph(_T("res/TemporaryMaterials/Title/UI_option.png"));
	_comandHandlle[5] = ResourceServer::LoadGraph(_T("res/TemporaryMaterials/Title/UI_quit_selelct.png"));

	_IsGameStart = false;
	//割れる処理の初期化
	 _modelHandle = ResourceServer::MV1LoadModel(_T("res/TemporaryMaterials/board.mv1"));
	 _currentTime = 0;
	 _IsBreak = false;
	 _frameSize = MV1GetFrameNum(_modelHandle);
	 _MoveVec = new VECTOR[_frameSize];
	 _rotVec = new VECTOR[_frameSize];

	 VECTOR breakPos = VGet(1020, -1080, 0);//割れる一の中心点
	 for (int i = 0; i < _frameSize; i++) {
	
		 VECTOR fPos = MV1GetFramePosition(_modelHandle, i);
		 _MoveVec[i] = VSub(fPos, breakPos);
		 _MoveVec[i] = VScale(_MoveVec[i], 0.03f);
		 _rotVec[i].x = rand() % 100 - 50;
		 _rotVec[i].y = rand() % 100 - 50;
		 _rotVec[i].z = rand() % 100 - 50;
		 _rotVec[i] = VScale(_rotVec[i], 0.001f);
	 }

	 MV1SetPosition(_modelHandle, VGet(0, 0, 0)); 
	 MV1SetScale(_modelHandle, VScale(VGet(1, 1, 1), 0.1));
	 SetCameraPositionAndTarget_UpVecY(VGet(0, 0, -1870), VGet(0, 0, 0));
	return true;
}

bool ModeTitle::Terminate() {
	base::Terminate();
	delete _MoveVec;
	delete _rotVec;
	return true;
}

void ModeTitle::SelectGameStart() {
	ModeServer::GetInstance()->Del(this);
	ModeServer::GetInstance()->Add(new ModeTest(), 1, "test");
};

void ModeTitle::SelectOption() {
	ModeServer::GetInstance()->Add(new ModePause(), 1, "Pause");
};

void ModeTitle::SelectGameEnd() {
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		global.exit_count = true;
	}
};

void ModeTitle::UpdateSelectItems(){
	int count = 0;

	//モード選択の切り替え
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_UP)) {
		count--;
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN)) {
		count++;
	}

	_modeCount += count;
	_modeCount = (_modeCount + 3) % 3;

	//モードの選択
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		int textureHandle = MakeGraph(1920, 1080);
		switch (_modeCount) {
		case 0:
			UpdateCrackedScreen();
			GetDrawScreenGraph( 0, 0, 1920, 1080, textureHandle);
			MV1SetTextureGraphHandle(_modelHandle, 0, textureHandle, false);
			_IsGameStart = true;
			_currentTime = GetNowCount();
			break;
		case 1:
			SelectOption();
			break;
		case 2:
			SelectGameEnd();
			break;
		}
	}
};

void ModeTitle::UpdateSelectToGameStart() {
	int nowTime = GetNowCount() - _currentTime;
	if (nowTime >= 1000) {
		UpdateCrackedScreen();
		if (nowTime >= 5000) {
			SelectGameStart();
		}
	}
};

void ModeTitle::UpdateCrackedScreen(){
	for (int i = 0; i < _frameSize; i++) {
		MATRIX matrix = MGetIdent();
		matrix = MGetRotX(_rotVec[i].x);
		matrix = MMult(matrix, MGetRotY(_rotVec[i].y));
		matrix = MMult(matrix, MGetRotZ(_rotVec[i].z));
		matrix = MMult(matrix, MGetTranslate(_MoveVec[i]));
		_MoveVec[i].y -= 2.5f;
		matrix = MMult(MV1GetFrameLocalMatrix(_modelHandle, i), matrix);
		MV1SetFrameUserLocalMatrix(_modelHandle, i, matrix);
	}
};

void ModeTitle::DrawTitleItems(){
	DrawFillBox(0, 0, 1920, 1080, GetColor(0, 0, 0));
	int x, y;
	//タイトルロゴの描画
	GetGraphSize(_titleLogo, &x, &y);
	//x = 1920 / 2 - x / 2;
	x = 840;
	DrawGraph(x, 145, _titleLogo, true);

	//それぞれの項目の描画
	GetGraphSize(_comandHandlle[0], &x, &y);
	int centerX, centerY;
	centerX = 1340;
	centerY = 555;

	for (int i = 0; i < 3; i++) {
		int handleNum = i;
		if (i == _modeCount) { handleNum += 3; }
		DrawExtendGraph(centerX, centerY + i * (72 + y / 2), centerX + x, centerY + y + i * (72 + y / 2), _comandHandlle[handleNum], true);
	}
};

void ModeTitle::DrawCrackedScreen() {
	MV1DrawModel(_modelHandle);
};

bool ModeTitle::Process() {
	base::Process();
	if (!_IsGameStart) {
		UpdateSelectItems();
	}
	else {
		UpdateSelectToGameStart();
	}

	return true;
}

bool ModeTitle::Render() {
	base::Render();
	if (!_IsGameStart) {
		DrawTitleItems();
	}
	else {
		DrawCrackedScreen();
	}
	return true;
}
