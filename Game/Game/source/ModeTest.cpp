#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeTest.h"

bool ModeTest::Initialize() {
	if (!base::Initialize()) { return false; }


	_camera = new Camera();

	_lightHandle[0] = CreateDirLightHandle(VGet(- 1, -1, -1));
	_lightHandle[1] = CreateDirLightHandle(VGet(1, 1, 1));

	_shadowHandle = MakeShadowMap(2048, 2048);

	_skySphere = MV1LoadModel(_T("res/SkySphere/skysphere.mv1"));
	_tile = MV1LoadModel(_T("res/TemporaryMaterials/stage_normal_01.mv1"));
	MV1SetPosition(_skySphere, VGet(0, 0, 0));
	MV1SetScale(_skySphere, VGet(3, 3, 3));
	MV1SetPosition(_tile, VGet(0, 0, 0));

	int playerModelHandle = MV1LoadModel("res/Character/cg_player_girl/cg_player_girl_TEST.mv1");
	_player = new Player(playerModelHandle, VGet(0, 0, 0));
	_player->SetNextExp("res/JsonFile/ExpList.json");

	_chain = new Chain();
	_chain->Init();

	//int objHandle = MV1LoadModel("res/Building/House_test_01.mv1");
	//for (int i = 0; i < 10; i++) {
	//	VECTOR v = VGet(rand() % 4000, 0.0f, rand() % 4000);
	//	v.x -= 2000.0f;
	//	v.z -= 2000.0f;

	//	House* building = new House();
	//	building->Init(MV1DuplicateModel(objHandle), v);

	//	_building.push_back(building);

	//}


	int objHandle = MV1LoadModel("res/Building/House/House_test_03.mv1");
	myJson json("Data/ObjectList/Stage_03.json");
	std::vector<std::string> loadName{ "House_Iron","House_Rock","House_Glass" };
	for (auto&& nameList : loadName) {
		std::vector<ModeTest::OBJECTDATA> objectData = LoadJsonObject(json._json, nameList);
		for (auto&& objectList : objectData) {
			House* building = new House();
			building->Init(MV1DuplicateModel(objHandle), objectList._pos, objectList._rotate, objectList._scale);
			_house.push_back(building);
		}
	}

	// タワー
	for (int i = 0; i < 10; i++) {
		VECTOR v = VGet(rand() % 4000, 0.0f, rand() % 4000);
		v.x -= 2000.0f;
		v.z -= 2000.0f;

		std::array<int, 3> towerModelHandle;
		towerModelHandle[0] = ResourceServer::MV1LoadModel("res/Building/Tower/test_Tower_01.mv1");
		towerModelHandle[1] = ResourceServer::MV1LoadModel("res/Building/Tower/test_Tower_02.mv1");
		towerModelHandle[2] = ResourceServer::MV1LoadModel("res/Building/Tower/test_Tower_03.mv1");

		Tower* tower = new Tower();
		tower->Init(towerModelHandle, v, VGet(0,0,0), VGet(1,1,1));

		_tower.push_back(tower);
	}


	int size = 100;
	int heartHandle[3];
	ResourceServer::LoadMultGraph("res/UI/UI_Heart", ".png", 3, heartHandle);
	ui[0] = new UIHeart(VGet(20, 20, 0), 3,heartHandle,2);
	//ui[0] = new UIHeart(VGet(0, 0, 0), "res/TemporaryMaterials/UI_Hp_01.png");
	ui[1] = new UIExpPoint(VGet(0, 150, 0), "res/TemporaryMaterials/UI_EXP_01.png");
	_gaugeUI[0] = new DrawGauge(0, 3, size, true);
	_gaugeUI[1] = new DrawGauge(0, 3, size, true);
	_gaugeHandle[0] = ResourceServer::LoadGraph(_T("res/UI/UI_Stamina_03.png"));
	_gaugeHandle[1] = ResourceServer::LoadGraph(_T("res/UI/UI_Stamina_02.png"));
	_gaugeHandle[2] = ResourceServer::LoadGraph(_T("res/UI/UI_Stamina_01.png"));
	_gaugeHandle[3] = ResourceServer::LoadGraph(_T("res/UI/UI_Stamina_04.png"));
	_sVib = new ScreenVibration();

	_enemyPool = new EnemyPool("res/JsonFile/EnemyData.json");
	_enemyPool->Create();

	_planeEffectManeger = new PlaneEffect::PlaneEffectManeger();
	ResourceServer::LoadMultGraph("res/TemporaryMaterials/split/test", ".png", 30, _effectSheet);
	//global._soundServer->DirectPlay("Stage03");
	global._soundServer->BgmFadeIn("Stage03", 2000);
	return true;
}

bool ModeTest::Terminate() {
	base::Terminate();
	return true;
}

std::vector<ModeTest::OBJECTDATA> ModeTest::LoadJsonObject(nlohmann::json json, std::string loadName) {
	nlohmann::json loadObject = json.at(loadName);
	std::vector<ModeTest::OBJECTDATA> _objectList;
	for (auto& list : loadObject) {
		OBJECTDATA object;
		list.at("objectName").get_to(object._name);
		list.at("translate").at("x").get_to(object._pos.x);
		list.at("translate").at("y").get_to(object._pos.z);
		list.at("translate").at("z").get_to(object._pos.y);
		list.at("rotate").at("x").get_to(object._rotate.x);
		list.at("rotate").at("y").get_to(object._rotate.z);
		list.at("rotate").at("z").get_to(object._rotate.y);
		list.at("scale").at("x").get_to(object._scale.x);
		list.at("scale").at("y").get_to(object._scale.z);
		list.at("scale").at("z").get_to(object._scale.y);
		//座標修正
		object._pos.x *= -1;
		//degree->radian
		object._rotate.x = object._rotate.x * DX_PI_F / 180;
		object._rotate.y = (object._rotate.y + 180) * DX_PI_F / 180;
		object._rotate.z = object._rotate.z * DX_PI_F / 180;
		_objectList.push_back(object);
	}
	return _objectList;
};

bool ModeTest::Process() {
	base::Process();

	global._timer->TimeElapsed();
	_sVib->UpdateScreenVibration();

	_player->Process(_camera->GetCamY());
	_chain->Process();
	_enemyPool->Process();

	for (int i = 0; i < sizeof(ui) / sizeof(ui[0]); i++) {
		ui[i]->Process();
	}

	bool isAttackState = _player->GetIsAttackState();
	bool isInvincible = _player->GetIsInvincible();
	VECTOR pPos = _player->GetPosition();

	VECTOR ibPos = _chain->GetBallPosition();
	float ibR = _chain->GetBallRadius();

	int ibPower = _chain->GetPower();

	for (auto itr = _house.begin(); itr != _house.end(); ++itr) {
		(*itr)->Process();
		
		if ((*itr)->GetUseCollision()) {
			OBB houseObb = (*itr)->GetOBBCollision();

			if (Collision3D::OBBSphereCol(houseObb, ibPos, ibR)) {
				if (isAttackState) {
					VECTOR vDir = VSub(houseObb.pos, pPos);
					(*itr)->ActivateBreakObject(true, vDir);
					_player->SetExp(50);
					global._soundServer->DirectPlay("OBJ_RockBreak");
				}
			}

			//エネミーがノックバック状態の時、建物にぶつかったら破壊する
			for (int i = 0; i < _enemyPool->ENEMY_MAX_SIZE; i++) {
				ENEMYTYPE enState = _enemyPool->GetEnemy(i)->GetEnemyState();
				float enR = _enemyPool->GetEnemy(i)->GetR();
				if (enState == ENEMYTYPE::DEAD) {
					VECTOR enPos = _enemyPool->GetEnemy(i)->GetCollisionPos();
					if (Collision3D::OBBSphereCol(houseObb, enPos, enR)) {
						VECTOR vDir = VSub(houseObb.pos, pPos);
						(*itr)->ActivateBreakObject(true, vDir);
						global._soundServer->DirectPlay("OBJ_RockBreak");
					}
				}
			}
		}
	}

	for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
		(*itr)->Process();
		
		if ((*itr)->GetCanBlast()) {
			VECTOR tPos = (*itr)->GetPos();
			Sphere tSphere = (*itr)->GetBottomSphereCollision();
			if (isAttackState) {
				Sphere ibSphere = { ibPos, ibR };
				if (Collision3D::SphereCol(ibSphere, tSphere)) {

					VECTOR vDir = VSub(tPos, pPos);
					(*itr)->SetBlast(vDir);
				}
			}
		}
	}



	for (int i = 0; i < _enemyPool->ENEMY_MAX_SIZE; i++) {
		if (isAttackState) {
			if (!_enemyPool->GetEnemy(i)->GetUse()) { continue; }
			VECTOR enPos = _enemyPool->GetEnemy(i)->GetCollisionPos();
			float enR = _enemyPool->GetEnemy(i)->GetR();

			if (Collision3D::SphereCol(ibPos, ibR, enPos, enR)) {
				VECTOR vDir = VSub(enPos, pPos);
				vDir = VNorm(vDir);
				_enemyPool->GetEnemy(i)->SetKnockBack(vDir, ibPower);
				PlaneEffect::BoardPolygon* effect = new PlaneEffect::BoardPolygon(VAdd(ibPos, VGet(0, 100, 0)), GetCameraBillboardMatrix(), 200, _effectSheet, 30, 1.0f / 60.0f * 1000.0f);
				_planeEffectManeger->LoadVertical(effect);
			}
		}


		// 敵とプレイヤーの当たり判定
		EnemyBase* enemy = _enemyPool->GetEnemy(i);
		Sphere eCol = { enemy->GetCollisionPos(), enemy->GetR() };
		Capsule pCol = _player->GetCollision();
		if (Collision3D::SphereCapsuleCol(eCol, pCol)) {
			if (!isInvincible) {
				_player->SetDamage();
			}
			VECTOR tmpPos = enemy->GetCollisionPos();
			tmpPos.y = 0.0f;

			VECTOR vDir = VSub(pCol.down_pos, tmpPos);
			vDir.y = 0.0f;
			float squareLength = VSquareSize(vDir);
			if (squareLength >= 0.0001f) {
				vDir = VNorm(vDir);
				tmpPos = VAdd(tmpPos, VScale(vDir, eCol.r + pCol.r));
				_player->SetPos(tmpPos);
			}
			enemy = nullptr;


		}
	}

	//空間分割を考えていないので無駄が多いです。
	for (int i = 0; i < _enemyPool->ENEMY_MAX_SIZE; i++) {
		VECTOR en1Pos = _enemyPool->GetEnemy(i)->GetCollisionPos();
		float en1R = _enemyPool->GetEnemy(i)->GetR();
		for (int j = 0; j < _enemyPool->ENEMY_MAX_SIZE; j++) {
			if (i == j) { continue; }
			VECTOR en2Pos = _enemyPool->GetEnemy(j)->GetCollisionPos();
			float en2R = _enemyPool->GetEnemy(j)->GetR();
			VECTOR dirVec = VSub(en2Pos, en1Pos);
			float length = VSize(dirVec);
			if (length <= en1R + en2R) {
				float pushLength = length - en1R - en2R;
				dirVec = VNorm(dirVec);
				_enemyPool->GetEnemy(i)->SetExtrusionPos(VScale(dirVec, pushLength));
			}
		}
	}


	if (XInput::GetInstance()->GetTrg(XINPUT_BUTTON_START)) {
		_enemyPool->Init();
		//_player->SetDamage();
		ModeServer::GetInstance()->Add(new ModePause(), 10, "Pause");
	}

	if (XInput::GetInstance()->GetKey(XINPUT_BUTTON_Y)) {
		if (nowParcent > 0) {
			nowParcent -= 1.0f / 120 * 100;
		}
	}
	else {
		if (nowParcent < 100) {
			nowParcent += 1.0f / 120 * 100;
		}
	}

	if (XInput::GetInstance()->GetTrg(XINPUT_BUTTON_BACK)) {
		_drawDebug = !_drawDebug;
		VECTOR pPos = _player->GetPosition();
		for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
			VECTOR hPos = (*itr)->GetPos();
			VECTOR tmpDir = VSub(hPos, pPos);
			tmpDir.y = 0.0f;
			(*itr)->SetBlast(tmpDir);
		}
	}

	if (_player->GetHP() <= 0) {
		global._soundServer->BgmFadeOut(2000);
		ModeServer::GetInstance()->Del(this);
		ModeServer::GetInstance()->Add(new ModeGameOver(), 1, "gameover");
	}

	VECTOR box_vec = ConvWorldPosToScreenPos(VAdd(_player->GetPosition(), VGet(0, 170, 0)));
	_gaugeUI[0]->Process(box_vec, _player->GetStamina(), _player->GetStaminaMax());
	_gaugeUI[1]->Process(box_vec, 100, 100);

	
	_planeEffectManeger->Update();
	_camera->Process(_player->GetPosition(), _tile);
	return true;
}

bool ModeTest::Render() {
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	MV1DrawModel(_skySphere);


	// ライト設定
	SetUseLighting(TRUE);

	// 0,0,0を中心に線を引く
	{
		float linelength = 1000.f;
		VECTOR v = { 0, 0, 0 };
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}

	//------------------------------------
	// シャドウマップの設定　
	// shadowCount 0 シャドウマップに描画 1 モデルの描画
	VECTOR lightDir = VGet(0, -1, 0);
	SetShadowMapLightDirection(_shadowHandle, lightDir);

	// シャドウマップに描画する範囲を設定
	// カメラの注視点を中心にする
	float length = 10000.f;
	VECTOR plPos = _player->GetPosition();
	SetShadowMapDrawArea(_shadowHandle, VAdd(plPos, VGet(-length, -1.0f, -length)), VAdd(plPos, VGet(length, length, length)));

	for (int shadowCount = 0; shadowCount < 2; shadowCount++) {
		// シャドウマップの設定
		if (shadowCount == 0) {
			// シャドウマップへの描画の準備
			ShadowMap_DrawSetup(_shadowHandle);
		}
		else if (shadowCount == 1) {
			// シャドウマップへの描画を終了
			ShadowMap_DrawEnd();

		}

		//-------------------------------------------------------------------------------------

		_player->Render();
		_enemyPool->Render();
		_chain->Render();
		//_chain->DrawDebugInfo();

		_planeEffectManeger->Render();
		//}
		for (auto itr = _house.begin(); itr != _house.end(); ++itr) {
			(*itr)->Render();
		}

		for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
			(*itr)->Render();
		}
	}

	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, _shadowHandle);
	MV1DrawModel(_tile);
	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);

	if (_drawDebug) {
		_player->DrawDebugInfo();
		_chain->DrawDebugInfo();
		for (auto itr = _house.begin(); itr != _house.end(); ++itr) {
			(*itr)->DrawDebugInfo();
		}
	}

	for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
		(*itr)->DrawDebugInfo();
	}
	SetUseZBuffer3D(FALSE);

	//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	for (int i = 0; i < sizeof(ui) / sizeof(ui[0]); i++) {
		ui[i]->Draw();
	}

	if (_player->GetStaminaRate() < 1.0f) {
		int handleNum = floorf(_player->GetStaminaRate() * 100.0f / 33.4f);
		_gaugeUI[1]->Draw(_gaugeHandle[handleNum]);
		_gaugeUI[0]->Draw(_gaugeHandle[3]);
	}

	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(62);
	DrawFormatString(45, 200, GetColor(0, 0, 0), "%d", _player->GetInstance()->GetNowLevel() + 1);
	SetFontSize(16);
	//for (auto itr = _buildingBase.begin(); itr != _buildingBase.end(); ++itr) {
	//	(*itr)->DrawDebugInfo();
	//}

	return true;
}
