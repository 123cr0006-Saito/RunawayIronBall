#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeGameOver.h"

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }

	

	_camera = new Camera();


	int playerModelHandle = MV1LoadModel("res/Character/cg_player_girl/cg_player_girl.mv1");
	_player = new Player(playerModelHandle, VGet(0,0,0));

	for (int i = 0; i < 10; i++) {
		_sla[i] = new SlaBlock(0, VGet(0, 0, 0), _player);
	}

	_chain = new Chain();
	_chain->Init();

	return true;
}

bool ModeGame::Terminate() {
	base::Terminate();
	return true;
}

bool ModeGame::Process() {
	base::Process();
	global.effect->ScreenVibration();
	_player->Process(_camera->GetCamY());
	_chain->Process(_player->GetRightHandPos());
	_camera->Process(_player->GetPosition());

	for (auto& katatumuri : _sla) {
		katatumuri->Process();
	}
	return true;
}

bool ModeGame::Render() {
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	// ƒ‰ƒCƒgÝ’è
	SetUseLighting(TRUE);
	//clsDx();
	
	// 0,0,0‚ð’†S‚Éü‚ðˆø‚­
	{
		float linelength = 1000.f;
		VECTOR v = { 0, 0, 0 };
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}
		
	_player->Render();
	_chain->Render();

	for (auto& katatumuri : _sla) {
		katatumuri->Render();
	}

	SetUseZBuffer3D(FALSE);
	return true;
}

