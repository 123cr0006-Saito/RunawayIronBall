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
	ui[0] = new UIHeart(VGet(0, 0, 0), "res/TemporaryMaterials/heart.png");
	ui[1] = new UIExpPoint(VGet(0, 150, 0), "res/TemporaryMaterials/UI_EXP_01.png");

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

	for (int i = 0; i < sizeof(ui) / sizeof(ui[0]); i++) {
		ui[i]->Process();
	}

	bool isSwinging = _player->GetIsSwing();
	VECTOR pPos = _player->GetPosition();

	VECTOR ibPos = _chain->GetBallPosition();
	float ibR = _chain->GetBallRadius();

	int ibPower = _chain->GetPower();

	for (auto itr = _building.begin(); itr != _building.end(); ++itr) {
		(*itr)->Process();

		OBB houseObb = (*itr)->GetOBBCollision();

		if (Collision3D::OBBSphereCol(houseObb, ibPos, ibR)) {
			if (isSwinging) {
				VECTOR vDir = VSub(houseObb.pos, pPos);
				(*itr)->ActivateBreakObject(true, vDir);
				_player->SetExp(50);
			}
		}
	}

	for (int i = 0; i < _enemyPool->ENEMY_MAX_SIZE; i++) {
		if (isSwinging) {
			VECTOR enPos = _enemyPool->GetEnemy(i)->GetCollisionPos();
			float enR = _enemyPool->GetEnemy(i)->GetR();

			if (Collision3D::SphereCol(ibPos, ibR, enPos, enR)) {
				VECTOR vDir = VSub(enPos, pPos);
				vDir = VNorm(vDir);
				_enemyPool->GetEnemy(i)->SetKnockBack(vDir, ibPower);
			}
		}
	}
	
	if (XInput::GetInstance()->GetTrg(XINPUT_BUTTON_START)) {
		_enemyPool->Init();
	}


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
	_enemyPool->Render();
	_chain->Render();
	_chain->DrawDebugInfo();

	for (auto itr = _building.begin(); itr != _building.end(); ++itr) {
		(*itr)->Render();
		(*itr)->DrawDebugInfo();
	}




	DrawSphere3D(_chain->GetBallPosition(), _chain->GetBallRadius(), 16, GetColor(255, 0, 0), GetColor(255, 0, 0), false);

	for (int i = 0; i < sizeof(ui) / sizeof(ui[0]); i++) {
		ui[i]->Draw();
	}

	SetUseZBuffer3D(FALSE);
	
	//for (auto itr = _buildingBase.begin(); itr != _buildingBase.end(); ++itr) {
	//	(*itr)->DrawDebugInfo();
	//}

	return true;
}

