#include "ModeLoading.h"
ModeLoading::ModeLoading(bool* flag) {
	_chara = nullptr;
	IsClear = flag;
};

bool ModeLoading::Initialize(){

	_chara = new LoadingPlayer();

	// 3‚c‹óŠÔ‚Ì‰æ–Ê‚Ì’†S“_‚ğˆÚ“®
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

		// 3‚c‹óŠÔ‚Ì‰æ–Ê‚Ì’†S“_‚ğˆÚ“®
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