#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeTest.h"

bool ModeTest::Initialize() {
	if (!base::Initialize()) { return false; }


	_camera = new Camera();

	_skySphere = MV1LoadModel(_T("res/SkySphere/skysphere.mv1"));
	MV1SetPosition(_skySphere, VGet(0, 0, 0));
	MV1SetScale(_skySphere, VGet(1, 1, 1));

	int playerModelHandle = MV1LoadModel("res/Character/cg_player_girl/test28swing4.mv1");
	_player = new Player(playerModelHandle, VGet(0, 0, 0));

	_chain = new Chain();
	_chain->Init();

	_buildingBase = new BuildingBase();
	int objHandle = MV1LoadModel("res/Test_BreakObject/centerpivot_2.mv1");
	_buildingBase->Init(objHandle);

	return true;
}

bool ModeTest::Terminate() {
	base::Terminate();
	return true;
}

bool ModeTest::Process() {
	base::Process();
	global._timer->TimeElapsed();

	_player->Process(_camera->GetCamY());
	_chain->Process(_player->GetRightHandPos());


	_buildingBase->Process();

	_camera->Process(_player->GetPosition());
	return true;
}

bool ModeTest::Render() {
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	// ƒ‰ƒCƒgÝ’è
	SetUseLighting(TRUE);
	//clsDx();

	MV1DrawModel(_skySphere);
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

	_buildingBase->Render();

	VECTOR ballPos = _chain->GetBallPosition();
	DrawSphere3D(ballPos, 130.0f, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), false);


	SetUseZBuffer3D(FALSE);
	return true;
}

