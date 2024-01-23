#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeTest.h"

bool ModeTest::Initialize() {
	if (!base::Initialize()) { return false; }


	_camera = new Camera();

	_skySphere = MV1LoadModel(_T("res/SkySphere/skysphere.mv1"));
	MV1SetPosition(_skySphere, VGet(0, 0, 0));
	MV1SetScale(_skySphere, VGet(3, 3, 3));

	int playerModelHandle = MV1LoadModel("res/Character/cg_player_girl/cg_player_girl_TEST.mv1");
	_player = new Player(playerModelHandle, VGet(0, 0, 0));
	_player->SetNextExp("res/JsonFile/ExpList.json");

	_chain = new Chain();
	_chain->Init();

	int objHandle = MV1LoadModel("res/Building/House_test_01.mv1");
	for (int i = 0; i < 10; i++) {
		VECTOR v = VGet(rand() % 4000, 0.0f, rand() % 4000);
		v.x -= 2000.0f;
		v.z -= 2000.0f;

		House* building = new House();
		building->Init(MV1DuplicateModel(objHandle), v);
		
		_building.push_back(building);

	}
	ui = new UIHeart(VGet(0,0,0),"res/TemporaryMaterials/heart.png");

	_enemyPool = new EnemyPool("res/JsonFile/EnemyData.json");
	_enemyPool->Create();
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
	_enemyPool->Process();
	bool isSwinging = _player->GetIsSwing();
	VECTOR pPos = _player->GetPosition();
	for (auto itr = _building.begin(); itr != _building.end(); ++itr) {
		(*itr)->Process();

		VECTOR ibPos = _chain->GetBallPosition();
		float ibR = _chain->GetBallRadius();

		OBB houseObb = (*itr)->GetOBBCollision();

		if (Collision3D::OBBSphereCol(houseObb, ibPos, ibR)) {
			if (isSwinging) {
				VECTOR vDir = VSub(houseObb.pos, pPos);
				(*itr)->ActivateBreakObject(true, vDir);
			}
		}
	}



	_camera->Process(_player->GetPosition());
	return true;
}

bool ModeTest::Render() {
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
	_enemyPool->Render();
	_chain->Render();
	_chain->DrawDebugInfo();
	ui->Draw();
	for (auto itr = _building.begin(); itr != _building.end(); ++itr) {
		(*itr)->Render();
		(*itr)->DrawDebugInfo();
	}




	DrawSphere3D(_chain->GetBallPosition(), _chain->GetBallRadius(), 16, GetColor(255, 0, 0), GetColor(255, 0, 0), false);



	SetUseZBuffer3D(FALSE);
	
	//for (auto itr = _buildingBase.begin(); itr != _buildingBase.end(); ++itr) {
	//	(*itr)->DrawDebugInfo();
	//}

	return true;
}

