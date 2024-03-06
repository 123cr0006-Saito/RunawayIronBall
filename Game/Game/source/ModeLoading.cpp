#include "ModeLoading.h"
ModeLoading::ModeLoading(bool* flag) {
	IsClear = flag;
};

bool ModeLoading::Initialize(){

	_chara = new LoadPlayerChara();

	// 3Ｄ空間の画面の中心点を移動
	int sizeX, sizeY, colorBit;
	GetScreenState(&sizeX, &sizeY, &colorBit);
	SetCameraScreenCenter(sizeX-500, sizeY-150);
	return true;
};

bool ModeLoading::Terminate(){
	IsClear = nullptr;
	delete _chara;
	return true;
};

bool ModeLoading::Process(){
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	if ((*IsClear)) {
		int time = 4 * 1000;
		ModeServer::GetInstance()->Add(new ModeFade(time,true),1000,"FadeIn");
		ModeServer::GetInstance()->Del(this);

		// 3Ｄ空間の画面の中心点を移動
		int sizeX, sizeY, colorBit;
		GetScreenState(&sizeX, &sizeY, &colorBit);
		SetCameraScreenCenter(sizeX / 2, sizeY / 2);

	}

	_chara->Process();

	VECTOR target = _chara->GetPos();
	VECTOR cameraPos = VAdd(target, VGet(0,0, -1000));
	SetCameraPositionAndTarget_UpVecY(cameraPos, target);

	return true;
};

bool ModeLoading::Render() {
	_chara->Render();
	return true;
};