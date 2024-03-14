
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

	_handleMap["title"] = ResourceServer::LoadGraph("Title",_T("res/ModeTitle/UI_Title.png"));
	_handleMap["start"] = ResourceServer::LoadGraph("Start",_T("res/ModeTitle/UI_Title_Play.png"));
	_handleMap["option"] = ResourceServer::LoadGraph("Option",_T("res/ModeTitle/UI_Title_Option.png"));
	_handleMap["quit"] = ResourceServer::LoadGraph("Quit",_T("res/ModeTitle/UI_Title_Quit.png"));

	_IsGameStart = false;
	//����鏈���̏�����
	 _modelHandle = ResourceServer::MV1LoadModel("Board",_T("res/TemporaryMaterials/board.mv1"));
	 _currentTime = 0;
	 _IsBreak = false;
	 _frameSize = MV1GetFrameNum(_modelHandle);
	 _MoveVec = NEW VECTOR[_frameSize];
	 _rotVec = NEW VECTOR[_frameSize];

	 VECTOR breakPos = VGet(0,0, 0);//������̒��S�_
	 for (int i = 0; i < _frameSize; i++) {
	
		 VECTOR fPos = MV1GetFramePosition(_modelHandle, i);
		 _MoveVec[i] = VSub(fPos, breakPos);
		 _MoveVec[i] = VScale(_MoveVec[i], 0.05f);
		 _rotVec[i].x = rand() % 100 - 50;
		 _rotVec[i].y = rand() % 100 - 50;
		 _rotVec[i].z = rand() % 100 - 50;
		 _rotVec[i] = VScale(_rotVec[i], 0.0002f);
	 }

	 MV1SetPosition(_modelHandle, VGet(0, 0, 0)); 
	// MV1SetScale(_modelHandle, VScale(VGet(1, 1, 1), 0.1));
	 SetCameraPositionAndTarget_UpVecY(VGet(0, 0, -1850), VGet(0, 0, 0));

	 // bgm�̐ݒ�
	 global._soundServer->DirectPlay("Title");

	 // �^�C�g���R�[��
	 std::string voiceNum[1] = {"IB_Title"};
	 global._soundServer->DirectPlay(voiceNum[0]);

	return true;
}

bool ModeTitle::Terminate() {
	base::Terminate();
	delete[] _MoveVec;
	delete[] _rotVec;
	_input = nullptr;
	_handleMap.clear();
	return true;
}

void ModeTitle::SelectGameStart() {
	ModeServer::GetInstance()->Del(this);
	ModeServer::GetInstance()->Add(NEW ModeScenario("Data/ScenarioData/Scenario01.csv"), 100, "Scenario");
	ModeServer::GetInstance()->Add(NEW ModeGame(), 1, "Game");
};

void ModeTitle::SelectOption() {
	ModeServer::GetInstance()->Add(NEW ModePause(), 10, "Pause");
};

void ModeTitle::SelectGameEnd() {
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		global.exit_count = true;
	}
};

void ModeTitle::UpdateSelectItems(){
	int count = 0;

	//���[�h�I���̐؂�ւ�
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_UP)) {
		count--;
		global._soundServer->DirectPlay("SE_Select");
	}
	else if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN)) {
		count++;
		global._soundServer->DirectPlay("SE_Select");
	}

	_modeCount += count;
	_modeCount = (_modeCount + 3) % 3;

	//���[�h�̑I��
	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		global._soundServer->DirectPlay("SE_Press");
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
	int handleX, handleY;
	//�^�C�g�����S�̕`��
	GetGraphSize(_handleMap["title"], &handleX, &handleY);
	//x = 1920 / 2 - x / 2;
	handleX = 840;
	DrawGraph(handleX, 145, _handleMap["title"], true);

	//���ꂼ��̍��ڂ̕`��
	
	int centerX, centerY;
	centerX = 1340;
	centerY = 555;

	std::array<std::string,3> _handleNameList = { "start","option","quit" };

	for (int i = 0; i < 3; i++) {
		int handleNum = i;
		float extRate = 1.0f;
		GetGraphSize(_handleMap[_handleNameList[handleNum]], &handleX, &handleY);
		if (i == _modeCount) { extRate = 1.1f; }
		DrawRotaGraph(centerX + handleX / 2, centerY + handleY / 2 + i * (72 + handleY / 2), extRate, 0.0f, _handleMap[_handleNameList[handleNum]], true);
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
