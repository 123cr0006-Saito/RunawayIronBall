#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeGameOver.h"

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }

	

	_camera = new Camera();


	int playerModelHandle = MV1LoadModel("res/Character/cg_player_girl/cg_player_girl.mv1");
	_player = new Player(playerModelHandle, VGet(0,0,0));


	return true;
}

bool ModeGame::Terminate() {
	base::Terminate();
	return true;
}

bool ModeGame::Process() {
	base::Process();
	
	_player->Process(_camera->GetCamY());
	_camera->Process(_player->GetPosition());
	return true;
}

bool ModeGame::Render() {
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	// ƒ‰ƒCƒgİ’è
	SetUseLighting(TRUE);
	//clsDx();
	

	
	_player->Render();

	// 0,0,0‚ğ’†S‚Éü‚ğˆø‚­
	{
		float linelength = 1000.f;
		VECTOR v = { 0, 0, 0 };
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}

	SetUseZBuffer3D(FALSE);
	return true;
}

