#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeGameOver.h"

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }


	_camera = new Camera();

	_skySphere = MV1LoadModel(_T("res/SkySphere/skysphere.mv1"));
	MV1SetPosition(_skySphere, VGet(0, 0, 0));
	MV1SetScale(_skySphere, VGet(1,1,1));

	int playerModelHandle = MV1LoadModel("res/Character/cg_player_girl/test28swing4.mv1");
	_player = new Player(playerModelHandle, VGet(0,0,0));

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
	global._timer->TimeElapsed();
	
	_player->Process(_camera->GetCamY());
	_chain->Process(_player->GetRightHandPos());
	_camera->Process(_player->GetPosition(),0);
	return true;
}

bool ModeGame::Render() {
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	// ライト設定
	SetUseLighting(TRUE);
	//clsDx();
	
	MV1DrawModel(_skySphere);
	// 0,0,0を中心に線を引く
	{
		float linelength = 1000.f;
		VECTOR v = { 0, 0, 0 };
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}
		
	_player->Render();
	_chain->Render();

	VECTOR ballPos = _chain->GetBallPosition();
	DrawSphere3D(ballPos, 130.0f, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), false);


	SetUseZBuffer3D(FALSE);
	return true;
}

