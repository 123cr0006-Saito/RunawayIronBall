#include "ModeLoading.h"
#include "ModeScenario.h"
bool ModeLoading::Initialize(){
	
	_chara = new LoadingPlayer();
	// 3Ｄ空間の画面の中心点を移動
	int sizeX, sizeY, colorBit;
	GetScreenState(&sizeX, &sizeY, &colorBit);
	SetCameraScreenCenter(sizeX - 500, sizeY - 150);
	global.ResourceLoad();
	return true;
};

bool ModeLoading::Terminate(){
	// 3Ｄ空間の画面の中心点を移動
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

	if (GetASyncLoadNum() <= 0) {
		int time = 4 * 1000;
		ModeServer::GetInstance()->Add(NEW ModeScenario("Data/ScenarioData/Scenario01.csv", 1), 1, "Scenario");
		ModeServer::GetInstance()->Add(new ModeFadeComeBack(time,this),100,"Fade");
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