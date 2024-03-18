#include "ModeGame.h"
#include "ModeClear.h"
#include "ModePause.h"
#include "ModeGameOver.h"
#include "ModeLoading.h"
#include "ModeFadeComeBack.h"
#include "ModeZoomCamera.h"
#include "ModeRotationCamera.h"
#include "ModeTutorial.h"

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }

	_collisionManager = NEW CollisionManager();
	_collisionManager->Init();

	_gate = nullptr;
	_stageNum = 1;
	IsLoading = true;
	IsTutorial = false;
	LoadFunctionThread = nullptr;

	_light = NEW Light("LightData");
	_timeLimit = NEW TimeLimit();
	SetTime();
	
	int resolution = 8192;
	_shadowHandle = MakeShadowMap(resolution, resolution);
	_skySphere = ResourceServer::Load("SkySpehe", "res/SkySphere/Skyspehre.mv1");
	_tile = ResourceServer::Load("Tile", "res/Tile/Stage_Base_01.mv1");
	_mountain = ResourceServer::Load("Mountain", "res/Mountain/Stage_mountain_02.mv1");
	MV1SetPosition(_skySphere, VGet(0, 0, 0));
	MV1SetScale(_skySphere, VScale(VGet(1,1,1),0.5f));
	MV1SetPosition(_tile, VGet(0, 0, 0));
	MV1SetPosition(_mountain, VGet(0, 0, 0));

	int playerModelHandle = ResourceServer::MV1LoadModel("Player", "res/Character/cg_player_girl/Cg_Player_Girl.mv1");
	_player = NEW Player();
	_player->Init(playerModelHandle, VGet(0, 0, 0));
	_camera = NEW Camera(_player->GetPosition());


	_classificationEffect = NEW ClassificationEffect();
	_effectManeger = NEW EffectManeger();


	{
		ResourceServer::LoadDivGraph("Gate", "res/TemporaryMaterials/FX_Hole_2D00_sheet.png", 43, 16, 3, 1200, 1200);
		ResourceServer::Load("Player", "res/Character/cg_player_girl/Cg_Player_Girl.mv1");
		ResourceServer::Load("IronBall", "res/Character/Cg_Iron_Ball/Cg_Iron_Ball.mv1");
		ResourceServer::Load("Chain", "res/Chain/Cg_Chain.mv1");
		ResourceServer::Load("GirlTexWhite", "res/Character/cg_player_girl/FlickerTexture.png");

		ResourceServer::Load("FX_3D_Level_Up", "res/Effekseer/FX_3D_Level_Up/FX_3D_Level_Up.efkefc");
		ResourceServer::Load("Stanp", "res/Effekseer/Attack/HorizontalThird.efkefc");
		ResourceServer::Load("Rotation", "res/Effekseer/FX_3D_Rotate_2/FX_3D_Rotate.efkefc");
		ResourceServer::Load("SlashR", "res/Effekseer/Slash/SlashRight.efkefc");
		ResourceServer::Load("SlashL", "res/Effekseer/Slash/SlashLeft.efkefc");
		ResourceServer::LoadMultGraph("split", "res/TemporaryMaterials/split/test", ".png", 30, _effectSheet);
		ResourceServer::LoadDivGraph("Dust", "res/TemporaryMaterials/FX_Dust_2D.png", 44, 20, 3, 1000, 1000);
		ResourceServer::LoadEffekseerEffect("Stanp", "res/Effekseer/Attack/HorizontalThird.efkefc");
		ResourceServer::LoadMultGraph("Tutorial", "res/Tutorial/Tutorial", ".png", 5);
	}

	_suppression = NEW Suppression();

	_enemyPool = NEW EnemyPool("res/JsonFile/EnemyData.json");
	_floor = NEW Floor();
	_fog = NEW Fog();
	// オブジェクトのデータの読み込み
	LoadObjectParam("BuildingtList.csv");
	// ステージのデータの読み込み
	std::string fileName = "Data/ObjectList/Stage_0" + std::to_string(_stageNum) + ".json";
	LoadStage(fileName);

	int size = 100;
	int heartHandle[3];
	ResourceServer::LoadMultGraph("Heart", "res/UI/Heart/UI_Heart", ".png", 3, heartHandle);
	ui[0] = NEW UIHeart(VGet(120, 20, 0), 3, heartHandle, 2);
	ui[1] = NEW UIExpPoint(VGet(100, 150, 0));
	ResourceServer::LoadMultGraph("Suppressiongauge", "res/UI/SuppressionGauge/SuppressionGauge", ".png", 3, heartHandle);
	ui[2] = NEW UISuppressionGauge(VGet(700, 100, 0), 3, heartHandle);
	ui[3] = NEW UITimeLimit(VGet(1600, 100, 0));
	_gaugeUI[0] = NEW DrawGauge(0, 3, size, true);
	_gaugeUI[1] = NEW DrawGauge(0, 3, size, true);
	_gaugeHandle[0] = ResourceServer::LoadGraph("Stamina03", ("res/UI/Stamina/UI_Stamina_03.png"));
	_gaugeHandle[1] = ResourceServer::LoadGraph("Stamina02", ("res/UI/Stamina/UI_Stamina_02.png"));
	_gaugeHandle[2] = ResourceServer::LoadGraph("Stamina01", ("res/UI/Stamina/UI_Stamina_01.png"));
	_gaugeHandle[3] = ResourceServer::LoadGraph("Stamina04", ("res/UI/Stamina/UI_Stamina_04.png"));
	_sVib = NEW ScreenVibration();

	ModeServer::GetInstance()->Add(NEW ModeRotationCamera(_stageNum), 50, "RotCamera");

	global._soundServer->BgmFadeIn("Stage03", 2000);


	return true;
}

bool ModeGame::Terminate() {
	base::Terminate();
	delete _collisionManager;
	delete _camera;
	delete _player;
	delete _sVib;
	delete _enemyPool;
	delete _suppression;
	delete _effectManeger;
	delete _classificationEffect;
	delete _light;
	delete _timeLimit;
	delete _floor;
	delete _fog;

	if (LoadFunctionThread != nullptr) {
		LoadFunctionThread->detach();
		delete LoadFunctionThread; LoadFunctionThread = nullptr;
	}

	if (_gate != nullptr) {
		delete _gate;
	}

	for (int i = 0; i < 4; i++) {
		delete ui[i];
	}

	for (int i = 0; i < 2; i++) {
		delete _gaugeUI[i];
	}

	for (auto&& house : _house) {
		delete house;
	}

	for (auto&& tower : _tower) {
		delete tower;
	}

	for (auto&& uObj : _uObj) {
		delete uObj;
	}

	return true;
}

void ModeGame::DeleteObject() {
	_collisionManager->ClearTreeAndList();
	if (_gate != nullptr) {
		delete _gate; _gate = nullptr;
	}

	_enemyPool->DeleteEnemy();

	_floor->Delete();

	for(auto itr = _house.begin(); itr != _house.end();++itr){
		delete (*itr);(*itr) = nullptr;
	}

	for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
		delete (*itr);(*itr) = nullptr;
	}

	for (auto itr = _uObj.begin(); itr != _uObj.end(); ++itr) {
		delete (*itr); (*itr) = nullptr;
	}

	_house.clear();
	_tower.clear();
	_uObj.clear();

	for(auto&& name : _objectName){
		ResourceServer::MV1DeleteModelAll(name);
	}

};

void ModeGame::SetTime() {
	int min[3] = { 10,10,10 };
	_timeLimit->SetTimeLimit(min[_stageNum - 1], 0);
};

std::vector<ModeGame::OBJECTDATA> ModeGame::LoadJsonObject(const myJson& json, std::string loadName) {
	nlohmann::json loadObject = json._json.at(loadName);
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
	std::vector<ObjectParam> paramList;
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
			ObjectParam objectParam;
			c += GetString(&p[c], &objectParam._name); // モデルの名前を取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &x); // obbのXサイズを取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &y); // obbのYサイズを取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &z); // obbのZサイズを取得
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &objectParam.isBreak); // 破壊可能化
			c += SkipSpace(&p[c], &p[size]); // 空白やコントロールコードをスキップする
			objectParam._size = VGet(x, y, z);

			_objectParam.push_back(objectParam);
		}
	}
	else {
		return false;
	}
	return true;
};

std::vector<std::string> ModeGame::LoadObjectName(std::string fileName) {
	std::vector<std::string> nameList;
	std::string filePath = "Data/LoadStageName/" + fileName + "/"  + fileName + "0" + std::to_string(_stageNum) + ".csv";
	// csvファイルを読み込む
	CFile file(filePath);
	// ファイルが開けた場合
	if (file.Success()) {
		int c = 0;
		const char* p = (const char*)file.Data();
		int size = file.Size();
		while (c < size) {
			std::string objectName;
			c += GetString(&p[c], '\r\n', &objectName,size - c); // モデルの名前を取得
			c += SkipSpace(&p[c], &p[size]); // 空白やコントロールコードをスキップする
			nameList.push_back(objectName);
		}
	}
	return nameList;
}

bool ModeGame::LoadStage(std::string fileName) {
	myJson* json = new myJson(fileName);
	int j = 0;

	_enemyPool->Create(*json,_stageNum);

	_floor->Create(*json, _stageNum);

	// タワー
	std::vector<ModeGame::OBJECTDATA> towerData = LoadJsonObject(*json, "Tower");
	for (auto&& towerParam : towerData) {

		std::array<int, 3> towerModelHandle;
		towerModelHandle[0] = ResourceServer::MV1LoadModel("Tower01", "res/Building/CG_OBJ_Tower/Tower_Under.mv1");
		towerModelHandle[1] = ResourceServer::MV1LoadModel("Tower02", "res/Building/CG_OBJ_Tower/Tower_Under.mv1");
		towerModelHandle[2] = ResourceServer::MV1LoadModel("Tower03", "res/Building/CG_OBJ_Tower/Tower_Top.mv1");

		Tower* tower = NEW Tower();
		tower->Init(towerModelHandle, towerParam._pos, towerParam._rotate, towerParam._scale);

		_tower.push_back(tower);
	}
	_objectName.push_back("Tower01");
	_objectName.push_back("Tower02");
	_objectName.push_back("Tower03");

	std::string buildingName = "Building";
	std::vector<std::string> objectName = LoadObjectName(buildingName);
	for (auto&& nameList : objectName) {

		auto itr = std::find_if(_objectParam.begin(), _objectParam.end(), [=](ObjectParam temp)
		{
				return temp._name == nameList;
		});

		std::vector<ModeGame::OBJECTDATA> objectData = LoadJsonObject(*json, nameList);
		std::string modelName = nameList;
		_objectName.push_back(modelName);
		std::string modelPath = "res/Building/" + modelName + "/" + modelName + ".mv1";
		for (auto&& object : objectData) {
			int objHandle = ResourceServer::MV1LoadModel(modelName, modelPath);
			if ((*itr).isBreak == 1) {
				// 壊れるオブジェクト
				House* building = NEW House();
				building->Init(objHandle, object._pos, object._rotate, object._scale, (*itr)._size);
				_house.push_back(building);
			}
			else {
				// 壊れないオブジェクト
				UnbreakableObject* uObj = NEW UnbreakableObject();
				uObj->Init(objHandle, object._pos, object._rotate, object._scale, (*itr)._size);
				_uObj.push_back(uObj);
			}
		}
	}

	// プレイヤーの座標指定
	nlohmann::json loadObject = (*json)._json.at("Player_Start_Position");
	VECTOR pos;
	loadObject.at(0).at("translate").at("x").get_to(pos.x);
	loadObject.at(0).at("translate").at("y").get_to(pos.z);
	loadObject.at(0).at("translate").at("z").get_to(pos.y);
	 pos.x *= -1;
	//_player->SetPos(pos);

	return true;
};

bool ModeGame::StageMutation() {
	// ロード開始
	IsLoading = false;
	// 中身がいらないものはdeleteする
	DeleteObject();
	// オブジェクトのデータの読み込み ファイル名は 1 から始まるので +1 する
	std::string fileName = "Data/ObjectList/Stage_0" + std::to_string(_stageNum) + ".json";
	LoadStage(fileName);
	// ロード終了

	IsLoading = true;

	// ロードスレッドを終了
	if (LoadFunctionThread != nullptr) {
		LoadFunctionThread->detach();
		delete LoadFunctionThread; LoadFunctionThread = nullptr;
	}
	return true;
}

bool ModeGame::Process() {
	base::Process();

	bool isAttackState = _player->GetEnabledIBAttackCollision();
	bool isInvincible = _player->GetIsInvincible();
	VECTOR pPos = _player->GetPosition();


	Sphere ibSphere = _player->GetIBCollision();

	int ibPower = _player->GetPower();

	
	Capsule plCol = _player->GetCollision();
	Sphere ibCol = _player->GetIBCollision();

	global._timer->TimeElapsed();
	_sVib->UpdateScreenVibration();

	_player->Process(_camera->GetCamY());
	_enemyPool->Process(isAttackState);
	_timeLimit->Process();
	_fog->Process();

	for (int i = 0; i < sizeof(ui) / sizeof(ui[0]); i++) {
		ui[i]->Process();
	}



	for (auto itr = _house.begin(); itr != _house.end(); ++itr) {
		(*itr)->Process();

		if ((*itr)->GetUseCollision()) {
			OBB houseObb = (*itr)->GetOBBCollision();

			//if (Collision3D::OBBSphereCol(houseObb, ibSphere)) {
			//	if (isAttackState) {
			//		VECTOR vDir = VSub(houseObb.pos, pPos);
			//		(*itr)->SetHit(vDir);
			//		_player->SetExp(50);
			//		global._soundServer->DirectPlay("OBJ_RockBreak");
			//		continue;
			//	}
			//}

			////エネミーがノックバック状態の時、建物にぶつかったら破壊する
			//houseObb.pos.y = 0; houseObb.length[1] = 0; //平面での当たり判定のため建物のy軸の長さを0にする]
			//int enemySize = _enemyPool->GetSize();
			//for (int i = 0; i < enemySize; i++) {
			//	EnemyBase* en = _enemyPool->GetEnemy(i);
			//	if (!en) { continue; }
			//	if (!en->GetUse()) { continue; }

			//	ENEMYTYPE enState = en->GetEnemyState();

			//	VECTOR enPos = en->GetCollisionPos(); enPos.y = 0;
			//	VECTOR hitPos = VGet(0, 0, 0);
			//	float enR = en->GetR();

			//	if (Collision3D::OBBSphereCol(houseObb, enPos, enR, &hitPos)) {
			//		if (enState == ENEMYTYPE::DEAD) {
			//			VECTOR vDir = VSub(houseObb.pos, pPos);
			//			(*itr)->SetHit(vDir);
			//			global._soundServer->DirectPlay("OBJ_RockBreak");
			//			continue;
			//		}
			//		else {
			//			VECTOR dirVec = VSub(enPos, hitPos);
			//			dirVec = VNorm(dirVec);
			//			VECTOR movePos = VAdd(hitPos, VScale(dirVec, enR));
			//			en->SetPos(movePos);
			//		}
			//	}
			//}
		}
	}

	for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
		(*itr)->Process();
	}

	//int enemySize = _enemyPool->GetSize();
	//for (int i = 0; i < enemySize; i++) {
	//	EnemyBase* enemy = _enemyPool->GetEnemy(i);
	//	if (!enemy) { continue; }
	//	if (!enemy->GetUse()) { continue; }
	//	if (isAttackState) {

	//		VECTOR enPos = enemy->GetCollisionPos();
	//		float enR = enemy->GetR();

	//		if (Collision3D::SphereCol(ibSphere.centerPos, ibSphere.r, enPos, enR)) {
	//			VECTOR vDir = VSub(enPos, pPos);
	//			vDir = VNorm(vDir);
	//			enemy->SetKnockBackAndDamage(vDir, ibPower);
	//		}
	//	}
	//}





	if (XInput::GetInstance()->GetTrg(XINPUT_BUTTON_START)) {
		//_enemyPool->Init();
		//_player->SetDamage();
		ModeServer::GetInstance()->Add(NEW ModePause(), 10, "Pause");
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
		ModeServer::GetInstance()->Add(NEW ModeGameOver(), 0, "GameOver");
		ModeServer::GetInstance()->Add(NEW ModeFadeComeBack(2500, "GameOver", 50), 100, "Fade");
	}

	VECTOR box_vec = ConvWorldPosToScreenPos(VAdd(_player->GetPosition(), VGet(0, 170, 0)));

	float ratio = 1.0f - _camera->GetTargetDistance() / _camera->GetMaxLength();
	_gaugeUI[0]->Process(box_vec, _player->GetStamina(), _player->GetStaminaMax(),ratio);
	_gaugeUI[1]->Process(box_vec, 100, 100,ratio);

	_collisionManager->Process();

	_player->AnimationProcess();


	_effectManeger->Update();
	_camera->Process(_player->GetPosition(), _tile);

	GateProcess();
	CreateTutorial();

	return true;
}


bool ModeGame::GateProcess() {

	//_suppression->SubSuppression(1);
	if (_suppression->GetIsRatio() && _stageNum < 3 ) {
		if (_gate == nullptr) {
			int handle[43];
			ResourceServer::LoadDivGraph("Gate", "res/TemporaryMaterials/FX_Hole_2D00_sheet.png", 43, 16, 3, 1200, 1200, handle);
			float time = 1.0f / 60.0f * 1000.0f;
			_gate = NEW Gate(VGet(0, 300, 0), 300, handle, 43, time, 1000);
			ModeServer::GetInstance()->Add(NEW ModeZoomCamera(), 10, "Camera");
		}
		_gate->Process();

		VECTOR pPos = _player->GetPosition();
		float pR = _player->GetCollision().r;
		VECTOR gPos = _gate->GetPos();
		float gR = _gate->GetR();

		// ゴールゲートの当たり判定
		if (Collision3D::SphereCol(pPos, pR, gPos, gR)) {
			_stageNum++;
			ModeServer::GetInstance()->Add(NEW ModeClear(this),100,"Clear");	
		}
	}
	return true;
};

void ModeGame::NewStage(){
	StageMutation();
	ModeServer::GetInstance()->Add(NEW ModeRotationCamera(_stageNum), 10, "RotCamera");
	SetTime();
};

void ModeGame::CreateTutorial() {
	if (!IsTutorial) {
		IsTutorial = true;
		if (_stageNum == 1) {
			int tutorialHandle[5];
			ResourceServer::LoadMultGraph("Tutorial", "res/Tutorial/Tutorial", ".png", 5, tutorialHandle);
			ModeServer::GetInstance()->Add(NEW ModeTutorial(tutorialHandle, 5), 10, "Tutorial");
		}
		else if (_stageNum == 4) {
			int tutorialHandle[5];
			ResourceServer::LoadMultGraph("Tutorial", "res/Tutorial/Tutorial", ".png", 5, tutorialHandle);
			ModeServer::GetInstance()->Add(NEW ModeTutorial(tutorialHandle, 5), 10, "Tutorial");
		}
	}
};

bool ModeGame::Render() {
	if (LoadFunctionThread)return true;

	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	MV1DrawModel(_skySphere);
	if (_stageNum < 3) {
		MV1DrawModel(_mountain);
	}
	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, _shadowHandle);
	MV1DrawModel(_tile);
	_floor->Render();
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

		for (auto itr = _uObj.begin(); itr != _uObj.end(); ++itr) {
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
		for (auto itr = _uObj.begin(); itr != _uObj.end(); ++itr) {
			(*itr)->DrawDebugInfo();
		}
		_collisionManager->DrawAreaIndex();
	}

	if (_gate != nullptr) {
		_gate->Draw();
	}
	
	SetUseZBuffer3D(FALSE);

	_effectManeger->Render();


	if (!ModeServer::GetInstance()->Search("RotCamera")) {
		for (int i = 0; i < sizeof(ui) / sizeof(ui[0]); i++) {
			ui[i]->Draw();
		}

		if (_player->GetStaminaRate() < 1.0f) {
			int handleNum = floorf(_player->GetStaminaRate() * 100.0f / 33.4f);
			_gaugeUI[1]->Draw(_gaugeHandle[handleNum]);
			_gaugeUI[0]->Draw(_gaugeHandle[3]);
		}
	}


	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//for (auto itr = _buildingBase.begin(); itr != _buildingBase.end(); ++itr) {
	//	(*itr)->DrawDebugInfo();
	//}

	return true;
}
