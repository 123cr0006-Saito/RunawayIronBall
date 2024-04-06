//----------------------------------------------------------------------
// @filename ModeLoading.cpp
// ��date: 2024/03/06
// ��author: saito ko
// @explanation
// loading���̃L�����N�^�[��S���Ȃǂ̏����E�`����s���N���X
//----------------------------------------------------------------------
#include "ModeLoading.h"
#include "ModeScenario.h"
bool ModeLoading::Initialize(){
	_chara = NEW LoadingPlayer();
	// 3�c��Ԃ̉�ʂ̒��S�_���ړ�
	int sizeX, sizeY, colorBit;
	GetScreenState(&sizeX, &sizeY, &colorBit);
	SetCameraScreenCenter(sizeX - 500, sizeY - 150);
	global.ResourceLoad();
	global.Init();
	return true;
};

bool ModeLoading::Terminate(){
	// 3�c��Ԃ̉�ʂ̒��S�_���ړ�
	int sizeX, sizeY, colorBit;
	GetScreenState(&sizeX, &sizeY, &colorBit);
	SetCameraScreenCenter(sizeX / 2, sizeY / 2);
	delete _chara; _chara = nullptr;
	return true;
};

bool ModeLoading::Process(){
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	ModeServer::GetInstance()->SkipRenderUnderLayer();

	if (GetASyncLoadNum() <= 0 && !ModeServer::GetInstance()->Search("Fade")) {
		ModeServer::GetInstance()->Add(NEW ModeScenario("Data/ScenarioData/Scenario01.csv",	1), 50, "Scenario");
		ModeServer::GetInstance()->Add(NEW ModeFadeComeBack(1000,this),1000,"Fade");
	}

	_chara->Process();

	VECTOR target = _chara->GetPos();
	VECTOR cameraPos = VAdd(target, VGet(0, 0, -1000));
	SetCameraPositionAndTarget_UpVecY(cameraPos, target);

	return true;
};

bool ModeLoading::Render() {
	_chara->Render();
	return true;
};