//----------------------------------------------------------------------
// @filename ModeGame.cpp
// ＠date: 2023/12/14
// ＠author: saito ko
// @explanation
// ボスバトル以外のステージを管理するクラス
//----------------------------------------------------------------------
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
	IsTutorial = false;

	_light = NEW Light("LightData");
	_timeLimit = NEW TimeLimit();
	
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
	_player->SetLevel(global._allExp);
	_gameOverCnt = 0;
	transitionGameOver = false;

	_camera = NEW Camera(_player->GetPosition());


	_classificationEffect = NEW ClassificationEffect();
	_effectManeger = NEW EffectManeger();

	_suppression = NEW Suppression();

	_enemyPool = NEW EnemyPool("res/JsonFile/EnemyData.json");
	_floor = NEW Floor();
	_fog = NEW Fog();
	// オブジェクトのデータの読み込み
	LoadObjectParam("BuildingtList.csv");
	// ステージのデータの読み込み
	std::string fileName = "Data/ObjectList/Stage_0" + std::to_string(global.GetStageNum()) + ".json";
	LoadStage(fileName);

	int size = 100;
	int heartHandle[3];
	ResourceServer::LoadMultGraph("Heart", "res/UI/Heart/UI_Heart", ".png", 3, heartHandle);
	ui[0] = NEW UIHeart(VGet(120, 20, 0), 3, heartHandle, 2);
	ui[1] = NEW UIExpPoint(VGet(100, 150, 0));
	int suppressionHandle[3];
	ResourceServer::LoadMultGraph("SuppressionGauge", "res/UI/SuppressionGauge/SuppressionGauge", ".png", 3, suppressionHandle);
	ui[2] = NEW UISuppressionGauge(VGet(600, 100, 0), 3, suppressionHandle);
	ui[3] = NEW UITimeLimit(VGet(1450, 30, 0));
	_gaugeUI[0] = NEW DrawGauge(0, 3, size, true);
	_gaugeUI[1] = NEW DrawGauge(0, 3, size, true);
	_gaugeHandle[0] = ResourceServer::LoadGraph("Stamina03", ("res/UI/Stamina/UI_Stamina_03.png"));
	_gaugeHandle[1] = ResourceServer::LoadGraph("Stamina02", ("res/UI/Stamina/UI_Stamina_02.png"));
	_gaugeHandle[2] = ResourceServer::LoadGraph("Stamina01", ("res/UI/Stamina/UI_Stamina_01.png"));
	_gaugeHandle[3] = ResourceServer::LoadGraph("Stamina04", ("res/UI/Stamina/UI_Stamina_04.png"));

	int stageNum = global.GetStageNum();
	if (stageNum < 4) {
		int min[3] = { 15,15,15 };
		TimeLimit::GetInstance()->SetTimeLimit(min[stageNum - 1], 0);
	}

	TimeLimit::GetInstance()->Stop();

	ModeServer::GetInstance()->Add(NEW ModeRotationCamera(global.GetStageNum()), 10, "RotCamera");

	return true;
}

bool ModeGame::Terminate() {
	base::Terminate();
	_suppression->ClearSuppression();
	delete _collisionManager;
	delete _camera;
	delete _player;
	delete _enemyPool;
	delete _suppression;
	delete _effectManeger;
	delete _classificationEffect;
	delete _light;
	delete _timeLimit;
	delete _floor;
	delete _fog;

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

	_house.clear();
	_tower.clear();
	_uObj.clear();

	for (auto&& name : _objectName) {
		ResourceServer::MV1DeleteModelAll(name);
	}

	return true;
}


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
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &objectParam._hp); // 耐久力
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &objectParam._exp); // 獲得経験値
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &objectParam._suppression); // 獲得経験値
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
	std::string filePath = "Data/LoadStageName/" + fileName + "/"  + fileName + "0" + std::to_string(global.GetStageNum()) + ".csv";
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
	myJson json = myJson(fileName);
	int j = 0;

	_enemyPool->Create(json,global.GetStageNum());

	_floor->Create(json, global.GetStageNum());

	// タワー
	std::vector<ModeGame::OBJECTDATA> towerData = LoadJsonObject(json, "Tower");
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

		std::vector<ModeGame::OBJECTDATA> objectData = LoadJsonObject(json, nameList);
		std::string modelName = nameList;
		_objectName.push_back(modelName);
		Suppression::GetInstance()->AddSuppression((*itr)._suppression * objectData.size());
		std::string modelPath = "res/Building/" + modelName + "/" + modelName + ".mv1";
		for (auto&& object : objectData) {
			int objHandle = ResourceServer::MV1LoadModel(modelName, modelPath);
			if ((*itr).isBreak == 1) {
				// 壊れるオブジェクト
				House* building = NEW House();
				building->Init(objHandle, nameList,object._pos, object._rotate, object._scale, (*itr)._size,(*itr)._hp, (*itr)._exp,(*itr)._suppression);
				_house.push_back(building);
			}
			else {
				// 壊れないオブジェクト
				UnbreakableObject* uObj = NEW UnbreakableObject();
				uObj->Init(objHandle, nameList,object._pos, object._rotate, object._scale, (*itr)._size);
				_uObj.push_back(uObj);
			}
		}
	}

	// プレイヤーの座標指定
	nlohmann::json loadObject = json._json.at("Player_Start_Position");
	VECTOR pos;
	loadObject.at(0).at("translate").at("x").get_to(pos.x);
	loadObject.at(0).at("translate").at("y").get_to(pos.z);
	pos.y = 0;
	 pos.x *= -1;
	_player->SetPos(pos);

	return true;
};

bool ModeGame::Process() {
	base::Process();
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();


	bool enabledIBAttackCollision = _player->GetEnabledIBAttackCollision();

	global._timer->TimeElapsed();

	_player->Process(_camera->GetCamY());
	_enemyPool->Process(enabledIBAttackCollision);
	_timeLimit->Process();
	_fog->Process(global.GetStageNum());

	// プレイヤーがステージ範囲外に出たら戻す
	VECTOR playerPos = _player->GetPosition();
	float stageWidth[3] = {STAGE_ONE_WIDTH,STAGE_TWO_WIDTH,STAGE_THREE_WIDTH};
	float stageDistance = stageWidth[global.GetStageNum() - 1] ;
	float playerDistance = VSquareSize(playerPos);
	if(playerDistance > stageDistance * stageDistance){
	    VECTOR vDir = VNorm(playerPos);
	    _player->SetPos(VScale(vDir,stageDistance));
	}

	for (int i = 0; i < sizeof(ui) / sizeof(ui[0]); i++) {
		ui[i]->Process();
	}

	for (auto itr = _house.begin(); itr != _house.end(); ++itr) {
		(*itr)->Process();
	}

	for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
		(*itr)->Process();
	}

	if (XInput::GetInstance()->GetTrg(XINPUT_BUTTON_START)) {
		ModeServer::GetInstance()->Add(NEW ModePause(), 10, "Pause");
	}

	if (XInput::GetInstance()->GetTrg(XINPUT_BUTTON_BACK)) {
		_drawDebug = !_drawDebug;
	}

	if (_player->GetHP() <= 0 || _timeLimit->GetTimeLimit() < 0) {
		global._soundServer->BgmFadeOut(2000);
		_gameOverCnt++;	
		if (!transitionGameOver && _gameOverCnt > 160) {
			ModeServer::GetInstance()->Add(NEW ModeGameOver(), 0, "GameOver");
			ModeServer::GetInstance()->Add(NEW ModeFadeComeBack(2000,this, "GameOver", 50), 100, "Fade");
			transitionGameOver = true;
		}
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
	
	if (_suppression->GetIsRatio() ) {
		if (_gate == nullptr) {
			VECTOR pos = VGet(0, 300, 0);
			int handle[43];
			ResourceServer::LoadDivGraph("Gate", "res/TemporaryMaterials/FX_Hole_2D00_sheet.png", 43, 16, 3, 1200, 1200, handle);
			float time = 1.0f / 60.0f * 1000.0f;
			if (global.GetStageNum() == 3) {
				pos = VGet(-6787.0f, 300.0f, 7486.0);
			}
			_gate = NEW Gate(pos, 300, handle, 43, time, 1000);
			ModeServer::GetInstance()->Add(NEW ModeZoomCamera(pos), 10, "Camera");
		}
		_gate->Process();

		VECTOR pPos = _player->GetPosition();
		float pR = _player->GetCollision().r;
		VECTOR gPos = _gate->GetPos();
		float gR = _gate->GetR();

		// ゴールゲートの当たり判定
		if (Collision3D::SphereCol(pPos, pR, gPos, gR) && !ModeServer::GetInstance()->Search("Clear")) {
			global.AddStageNum();
			ModeServer::GetInstance()->Del(this);
			ModeServer::GetInstance()->Add(NEW ModeClear(_timeLimit->GetElapsedTime(),_timeLimit->GetStartTime()),100,"Clear");	
		}
	}
	return true;
};

void ModeGame::CreateTutorial() {
	if (!IsTutorial) {
		IsTutorial = true;
		if (global.GetStageNum() == 1) {
			int tutorialHandle[5];
			ResourceServer::LoadMultGraph("Tutorial", "res/Tutorial/Tutorial", ".png", 5, tutorialHandle);
			ModeServer::GetInstance()->Add(NEW ModeTutorial(tutorialHandle, 5), 10, "Tutorial");
		}
	}
};

bool ModeGame::Render() {

	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	MV1DrawModel(_skySphere);
	if (global.GetStageNum() < 3) {
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

		for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
			(*itr)->Render();
		}

		for (auto itr = _uObj.begin(); itr != _uObj.end(); ++itr) {
			(*itr)->Render();
		}
		for (auto itr = _house.begin(); itr != _house.end(); ++itr) {
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

	return true;
}
