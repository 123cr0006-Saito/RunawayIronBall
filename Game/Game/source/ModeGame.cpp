#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeTest.h"

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }

	_light = new Light("LightData");
	
	int resolution = 8192;
	_shadowHandle = MakeShadowMap(resolution, resolution);

	_skySphere = MV1LoadModel(_T("res/SkySphere/skysphere.mv1"));
	_tile = MV1LoadModel(_T("res/TemporaryMaterials/stage_normal_01.mv1"));
	MV1SetPosition(_skySphere, VGet(0, 0, 0));
	MV1SetScale(_skySphere, VGet(3, 3, 3));
	MV1SetPosition(_tile, VGet(0, 0, 0));

	int playerModelHandle = ResourceServer::MV1LoadModel("Player", "res/Character/cg_player_girl/cg_player_girl_TEST_Ver.2.mv1");
	_player = new Player(playerModelHandle, VGet(0, 0, 0));
	_camera = new Camera(_player->GetPosition());


	_classificationEffect = new ClassificationEffect();
	_effectManeger = new EffectManeger();

	{

		ResourceServer::LoadMultGraph("split", "res/TemporaryMaterials/split/test", ".png", 30, _effectSheet);
		int handle[44];
		ResourceServer::LoadDivGraph("Dust", "res/TemporaryMaterials/FX_Dust_2D.png", 44, 20, 3, 1000, 1000, handle);
		ResourceServer::LoadEffekseerEffect("Stanp", "res/Effekseer/Attack/HorizontalThird.efkefc");
	}

	_suppression = new Suppression();

	_enemyPool = new EnemyPool("res/JsonFile/EnemyData.json");

	// オブジェクトのデータの読み込み
	LoadObjectParam("BuildingtList.csv");
	// ステージのデータの読み込み
	LoadStage("Data/ObjectList/Stage_03.json");

	int size = 100;
	int heartHandle[3];
	ResourceServer::LoadMultGraph("Heart", "res/UI/UI_Heart", ".png", 3, heartHandle);
	ui[0] = new UIHeart(VGet(20, 20, 0), 3, heartHandle, 2);
	ui[1] = new UIExpPoint(VGet(0, 150, 0), "res/TemporaryMaterials/UI_EXP_01.png");
	ResourceServer::LoadMultGraph("Suppressiongauge", "res/TemporaryMaterials/SuppressionGauge/suppressiongauge", ".png", 3, heartHandle);
	ui[2] = new UISuppressionGauge(VGet(700, 100, 0), 3, heartHandle);
	_gaugeUI[0] = new DrawGauge(0, 3, size, true);
	_gaugeUI[1] = new DrawGauge(0, 3, size, true);
	_gaugeHandle[0] = ResourceServer::LoadGraph("Stamina03", _T("res/UI/UI_Stamina_03.png"));
	_gaugeHandle[1] = ResourceServer::LoadGraph("Stamina02", _T("res/UI/UI_Stamina_02.png"));
	_gaugeHandle[2] = ResourceServer::LoadGraph("Stamina01", _T("res/UI/UI_Stamina_01.png"));
	_gaugeHandle[3] = ResourceServer::LoadGraph("Stamina04", _T("res/UI/UI_Stamina_04.png"));
	_sVib = new ScreenVibration();



	//global._soundServer->DirectPlay("Stage03");
	global._soundServer->BgmFadeIn("Stage03", 2000);



	TowerParts::InitBlastTowerParts();

	return true;
}

bool ModeGame::Terminate() {
	base::Terminate();

	delete _light;
	return true;
}

std::vector<ModeGame::OBJECTDATA> ModeGame::LoadJsonObject(nlohmann::json json, std::string loadName) {
	nlohmann::json loadObject = json.at(loadName);
	std::vector<ModeGame::OBJECTDATA> _objectList;
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

bool ModeGame::LoadObjectParam(std::string fileName) {
	std::vector<std::tuple<std::string, VECTOR, bool> > paramList;
	std::string filePath = "Data/BuildingData/" + fileName;
	// csvファイルを読み込む
	CFile file(filePath);
	// ファイルが開けた場合
	if (file.Success()) {
		int c = 0;
		const char* p = (const char*)file.Data();
		int size = file.Size();
		while (c < size) {
			std::string modelName;
			int x, y, z;
			int IsBreaking;
			c += GetString(&p[c], &modelName); // モデルの名前を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &x); // obbのXサイズを取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &y); // obbのYサイズを取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &z); // obbのZサイズを取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &IsBreaking); // 破壊可能化
			c += SkipSpace(&p[c], &p[size]); // 空白やコントロールコードをスキップする
			_objectParam.push_back(std::make_tuple(modelName, VGet(x, y, z), IsBreaking));
		}
	}
	else {
		return false;
	}
	return true;
};

bool ModeGame::LoadStage(std::string fileName) {
	myJson json(fileName);

	_enemyPool->Create(json);

	//int objHandle = MV1LoadModel("res/Building/House/House_test_03.mv1");
	//int objHandle = MV1LoadModel("res/Building/TrafficLight/cg_object_shingou.mv1");
	//int objHandle = MV1LoadModel("res/Building/Pole/cg_object_denchu.mv1");
	//int objHandle = MV1LoadModel("res/Building/StoneLantern/cg_object_tourou.mv1");

	for (auto&& nameList : _objectParam) {
		std::vector<ModeGame::OBJECTDATA> objectData = LoadJsonObject(json._json, std::get<0>(nameList));
		std::string modelName = "res/Building/" + std::get<0>(nameList) + "/" + std::get<0>(nameList) + ".mv1";
		int objHandle = MV1LoadModel(modelName.c_str());
		for (auto&& object : objectData) {
			if (std::get<2>(nameList) == 1) {
				// 壊れるオブジェクト
				House* building = new House();
				building->Init(MV1DuplicateModel(objHandle), object._pos, object._rotate, object._scale, std::get<1>(nameList));
				_house.push_back(building);
			}
			else {
				// 壊れないオブジェクト
			}
		}
	}

	// タワー
	for (int i = 0; i < 5; i++) {
		VECTOR v = VGet(rand() % 8000, 0.0f, rand() % 8000);
		v.x -= 4000.0f;
		v.z -= 4000.0f;

		std::array<int, 3> towerModelHandle;
		towerModelHandle[0] = ResourceServer::MV1LoadModel("Tower01", "res/Building/CG_OBJ_Tower/CG_OBJ_Tower_Under.mv1");
		towerModelHandle[1] = ResourceServer::MV1LoadModel("Tower02", "res/Building/CG_OBJ_Tower/CG_OBJ_Tower_Under.mv1");
		towerModelHandle[2] = ResourceServer::MV1LoadModel("Tower03", "res/Building/CG_OBJ_Tower/CG_OBJ_Tower_Top.mv1");

		Tower* tower = new Tower();
		tower->Init(towerModelHandle, v, VGet(0, 0, 0), VGet(1, 1, 1));

		_tower.push_back(tower);
	}

	return true;
};

bool ModeGame::Process() {
	base::Process();

	global._timer->TimeElapsed();
	_sVib->UpdateScreenVibration();

	_player->Process(_camera->GetCamY());
	_enemyPool->Process();

	for (int i = 0; i < sizeof(ui) / sizeof(ui[0]); i++) {
		ui[i]->Process();
	}

	bool isAttackState = _player->GetEnabledIBAttackCollision();
	bool isInvincible = _player->GetIsInvincible();
	VECTOR pPos = _player->GetPosition();


	Sphere ibSphere = _player->GetIBCollision();

	int ibPower = _player->GetPower();


	Capsule plCol = _player->GetCollision();
	Sphere ibCol = _player->GetIBCollision();

	for (auto itr = _house.begin(); itr != _house.end(); ++itr) {
		(*itr)->Process();

		if ((*itr)->GetUseCollision()) {
			OBB houseObb = (*itr)->GetOBBCollision();

			if (Collision3D::OBBSphereCol(houseObb, ibSphere)) {
				if (isAttackState) {
					VECTOR vDir = VSub(houseObb.pos, pPos);
					(*itr)->ActivateBreakObject(true, vDir);
					_player->SetExp(50);
					global._soundServer->DirectPlay("OBJ_RockBreak");
					continue;
				}
			}

			//エネミーがノックバック状態の時、建物にぶつかったら破壊する
			houseObb.pos.y = 0; houseObb.length[1] = 0; //平面での当たり判定のため建物のy軸の長さを0にする]
			for (int i = 0; i < _enemyPool->ENEMY_MAX_SIZE; i++) {
				EnemyBase* en = _enemyPool->GetEnemy(i);
				if (!en) { continue; }
				if (!en->GetUse()) { continue; }

				ENEMYTYPE enState = en->GetEnemyState();

				VECTOR enPos = en->GetCollisionPos(); enPos.y = 0;
				VECTOR hitPos = VGet(0, 0, 0);
				float enR = en->GetR();

				if (Collision3D::OBBSphereCol(houseObb, enPos, enR, &hitPos)) {
					if (enState == ENEMYTYPE::DEAD) {
						VECTOR vDir = VSub(houseObb.pos, pPos);
						(*itr)->ActivateBreakObject(true, vDir);
						global._soundServer->DirectPlay("OBJ_RockBreak");
						continue;
					}
					else {
						VECTOR dirVec = VSub(enPos, hitPos);
						dirVec = VNorm(dirVec);
						VECTOR movePos = VAdd(hitPos, VScale(dirVec, enR));
						en->SetPos(movePos);
					}
				}
			}

			for (auto tpItr = TowerParts::_blastTowerParts.begin(); tpItr != TowerParts::_blastTowerParts.end(); ++tpItr) {
				Sphere tpSphere = (*tpItr)->GetSphereCollision();
				if (Collision3D::OBBSphereCol(houseObb, tpSphere)) {
					VECTOR vDir = VSub(houseObb.pos, tpSphere.centerPos);
					(*itr)->ActivateBreakObject(true, vDir);
					continue;
				}
			}

			// プレイヤーの押出
			VECTOR hitPos = VGet(0, 0, 0);
			VECTOR pPos = _player->GetPosition();
			float pPosY = pPos.y;
			pPos.y = 0;
			float pR = _player->GetCollision().r;
			if (Collision3D::OBBSphereCol(houseObb, pPos, pR, &hitPos)) {
				VECTOR dirVec = VSub(pPos, hitPos);
				dirVec = VNorm(dirVec);
				VECTOR movePos = VAdd(hitPos, VScale(dirVec, pR));
				_player->SetPos(movePos);

			}
		}
	}

	for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
		(*itr)->Process();

		if ((*itr)->GetUse()) {
			VECTOR tPos = (*itr)->GetPos();
			Sphere tSphere = (*itr)->GetBottomSphereCollision();
			if ((*itr)->GetCanBlast()) {
				if (isAttackState) {
					if (Collision3D::SphereCol(ibSphere, tSphere)) {

						VECTOR vDir = VSub(tPos, pPos);
						(*itr)->SetBlast(vDir);
					}
				}
			}

			// エネミーの押出処理
			for (int i = 0; i < _enemyPool->ENEMY_MAX_SIZE; i++) {
				EnemyBase* en = _enemyPool->GetEnemy(i);
				if (!en) { continue; }
				if (!en->GetUse()) { continue; }

				float tR = tSphere.r;
				VECTOR enPos = en->GetCollisionPos();
				float enR = en->GetR();

				enPos.y = 0;
				tSphere.centerPos.y = 0;

				VECTOR vDir = VSub(enPos, tSphere.centerPos);
				if (VSize(vDir) <= enR + tR) {
					float len = (enR + tR) - VSize(vDir);
					vDir = VNorm(vDir);
					en->SetPos(VAdd(enPos, VScale(vDir, len)));
				}
				en = nullptr;
			}

			// プレイヤーの押出
			VECTOR pColPos = _player->GetPosition(); pColPos.y = 0;
			float pR = _player->GetCollision().r;
			float tR = tSphere.r;


			VECTOR vDir = VSub(pColPos, tSphere.centerPos);
			if (VSize(vDir) <= pR + tR) {
				float len = (pR + tR) - VSize(vDir);
				vDir = VNorm(vDir);
				_player->SetPos(VAdd(pColPos, VScale(vDir, len)));
			}
		}
	}


	for (int i = 0; i < _enemyPool->ENEMY_MAX_SIZE; i++) {
		EnemyBase* enemy = _enemyPool->GetEnemy(i);
		if (!enemy) { continue; }
		if (!enemy->GetUse()) { continue; }
		if (isAttackState) {

			VECTOR enPos = enemy->GetCollisionPos();
			float enR = enemy->GetR();

			if (Collision3D::SphereCol(ibSphere.centerPos, ibSphere.r, enPos, enR)) {
				VECTOR vDir = VSub(enPos, pPos);
				vDir = VNorm(vDir);
				enemy->SetKnockBack(vDir, ibPower);
			}
		}
		// 敵とプレイヤーの当たり判定

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
		EnemyBase* en = _enemyPool->GetEnemy(i);
		if (!en) { continue; }
		if (!en->GetUse()) { continue; }

		VECTOR en1Pos = en->GetCollisionPos();
		float en1R = en->GetR();
		for (int j = 0; j < _enemyPool->ENEMY_MAX_SIZE; j++) {
			if (i == j) { continue; }

			EnemyBase* en = _enemyPool->GetEnemy(j);
			if (!en) { continue; }
			if (!en->GetUse()) { continue; }

			VECTOR en2Pos = en->GetCollisionPos();
			float en2R = en->GetR();
			VECTOR dirVec = VSub(en2Pos, en1Pos);
			float length = VSize(dirVec);
			if (length <= en1R + en2R) {
				float pushLength = (en1R + en2R) - length;
				dirVec = VNorm(dirVec);
				en->SetExtrusionPos(VScale(dirVec, pushLength));
			}
		}
	}



	if (XInput::GetInstance()->GetTrg(XINPUT_BUTTON_START)) {
		//_enemyPool->Init();
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
		//VECTOR pPos = _player->GetPosition();
		//for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
		//	
		//	VECTOR hPos = (*itr)->GetPos();
		//	VECTOR tmpDir = VSub(hPos, pPos);
		//	tmpDir.y = 0.0f;
		//	(*itr)->SetBlast(tmpDir);
		//}
	}

	if (_player->GetHP() <= 0) {
		global._soundServer->BgmFadeOut(2000);
		ModeServer::GetInstance()->Del(this);
		ModeServer::GetInstance()->Add(new ModeGameOver(), 1, "gameover");
	}

	VECTOR box_vec = ConvWorldPosToScreenPos(VAdd(_player->GetPosition(), VGet(0, 170, 0)));

	float ratio = 1.0f - _camera->GetTargetDistance() / _camera->GetMaxLength();
	_gaugeUI[0]->Process(box_vec, _player->GetStamina(), _player->GetStaminaMax(),ratio);
	_gaugeUI[1]->Process(box_vec, 100, 100,ratio);

	_player->AnimationProcess();

	TowerParts::CheckFinishedBlastTowerParts();

	_effectManeger->Update();
	_camera->Process(_player->GetPosition(), _tile);
	return true;
}


bool ModeGame::GateProcess() {
	return true;
};// ゴールゲートの処理


bool ModeGame::Render() {
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	MV1DrawModel(_skySphere);

	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, _shadowHandle);
	MV1DrawModel(_tile);
	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);


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
	VECTOR lightDir = VGet(-0.5f, -1.0f, -0.5f);
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
		//_chain->DrawDebugInfo();

		
		//}
		for (auto itr = _house.begin(); itr != _house.end(); ++itr) {
			(*itr)->Render();
		}

		for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
			(*itr)->Render();
		}
	}


	if (_drawDebug) {
		_player->DrawDebugInfo();
		for (auto itr = _house.begin(); itr != _house.end(); ++itr) {
			(*itr)->DrawDebugInfo();
		}
		for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
			(*itr)->DrawDebugInfo();
		}
	}

	
	SetUseZBuffer3D(FALSE);

	_effectManeger->Render();


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
