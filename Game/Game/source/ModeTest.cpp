//#include "AppFrame.h"
//#include "ApplicationMain.h"
//#include "ModeTest.h"
//
//bool ModeTest::Initialize() {
//	if (!base::Initialize()) { return false; }
//
//	int resolution = 8192;
//	_shadowHandle = MakeShadowMap(resolution, resolution);
//
//	_skySphere = MV1LoadModel(_T("res/SkySphere/skysphere.mv1"));
//	_tile = MV1LoadModel(_T("res/TemporaryMaterials/stage_normal_01.mv1"));
//	MV1SetPosition(_skySphere, VGet(0, 0, 0));
//	MV1SetScale(_skySphere, VGet(3, 3, 3));
//	MV1SetPosition(_tile, VGet(0, 0, 0));
//
//	int playerModelHandle = ResourceServer::MV1LoadModel("Player", "res/Character/cg_player_girl/cg_player_girl_TEST_Ver.2.mv1");
//	_player = NEW Player(playerModelHandle, VGet(0, 0, 0));
//	_camera = NEW Camera(_player->GetPosition());
//
//
//	_classificationEffect = NEW ClassificationEffect();
//	_effectManeger = NEW EffectManeger();
//
//	{
//
//		ResourceServer::LoadMultGraph("split", "res/TemporaryMaterials/split/test", ".png", 30, _effectSheet);
//		int handle[44];
//		ResourceServer::LoadDivGraph("Dust", "res/TemporaryMaterials/FX_Dust_2D.png", 44, 20, 3, 1000, 1000, handle);
//		ResourceServer::LoadEffekseerEffect("Stanp", "res/Effekseer/Attack/HorizontalThird.efkefc");
//	}
//
//	_suppression = NEW Suppression();
//
//
//
//	int size = 100;
//	int heartHandle[3];
//	ResourceServer::LoadMultGraph("Heart", "res/UI/UI_Heart", ".png", 3, heartHandle);
//	ui[0] = NEW UIHeart(VGet(20, 20, 0), 3, heartHandle, 2);
//	ui[1] = NEW UIExpPoint(VGet(0, 150, 0), "res/TemporaryMaterials/UI_EXP_01.png");
//	ResourceServer::LoadMultGraph("Suppressiongauge", "res/TemporaryMaterials/SuppressionGauge/suppressiongauge", ".png", 3, heartHandle);
//	ui[2] = NEW UISuppressionGauge(VGet(700, 100, 0), 3, heartHandle);
//	_gaugeUI[0] = NEW DrawGauge(0, 3, size, true);
//	_gaugeUI[1] = NEW DrawGauge(0, 3, size, true);
//	_gaugeHandle[0] = ResourceServer::LoadGraph("Stamina03", _T("res/UI/UI_Stamina_03.png"));
//	_gaugeHandle[1] = ResourceServer::LoadGraph("Stamina02", _T("res/UI/UI_Stamina_02.png"));
//	_gaugeHandle[2] = ResourceServer::LoadGraph("Stamina01", _T("res/UI/UI_Stamina_01.png"));
//	_gaugeHandle[3] = ResourceServer::LoadGraph("Stamina04", _T("res/UI/UI_Stamina_04.png"));
//	_sVib = NEW ScreenVibration();
//
//
//
//	//global._soundServer->DirectPlay("Stage03");
//	global._soundServer->BgmFadeIn("Stage03", 2000);
//	TowerParts::InitBlastTowerParts();
//
//
//	_boss = NEW Boss();
//	_boss->LoadModel();
//	_boss->Init(VGet(0, 0, 500));
//
//	return true;
//}
//
//bool ModeTest::Terminate() {
//	base::Terminate();
//
//	return true;
//}
//
//std::vector<ModeTest::OBJECTDATA> ModeTest::LoadJsonObject(nlohmann::json json, std::string loadName) {
//	nlohmann::json loadObject = json.at(loadName);
//	std::vector<ModeTest::OBJECTDATA> _objectList;
//	for (auto& list : loadObject) {
//		OBJECTDATA object;
//		list.at("objectName").get_to(object._name);
//		list.at("translate").at("x").get_to(object._pos.x);
//		list.at("translate").at("y").get_to(object._pos.z);
//		list.at("translate").at("z").get_to(object._pos.y);
//		list.at("rotate").at("x").get_to(object._rotate.x);
//		list.at("rotate").at("y").get_to(object._rotate.z);
//		list.at("rotate").at("z").get_to(object._rotate.y);
//		list.at("scale").at("x").get_to(object._scale.x);
//		list.at("scale").at("y").get_to(object._scale.z);
//		list.at("scale").at("z").get_to(object._scale.y);
//		//座標修正
//		object._pos.x *= -1;
//		//degree->radian
//		object._rotate.x = object._rotate.x * DX_PI_F / 180;
//		object._rotate.y = (object._rotate.y + 180) * DX_PI_F / 180;
//		object._rotate.z = object._rotate.z * DX_PI_F / 180;
//		_objectList.push_back(object);
//	}
//	return _objectList;
//};
//
//bool ModeTest::LoadObjectParam(std::string fileName) {
//	std::vector<std::tuple<std::string, VECTOR, bool> > paramList;
//	std::string filePath = "Data/BuildingData/" + fileName;
//	// csvファイルを読み込む
//	CFile file(filePath);
//	// ファイルが開けた場合
//	if (file.Success()) {
//		int c = 0;
//		const char* p = (const char*)file.Data();
//		int size = file.Size();
//		while (c < size) {
//			std::string modelName;
//			int x, y, z;
//			int IsBreaking;
//			c += GetString(&p[c], &modelName); // モデルの名前を取得
//			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &x); // obbのXサイズを取得
//			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &y); // obbのYサイズを取得
//			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &z); // obbのZサイズを取得
//			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &IsBreaking); // 破壊可能化
//			c += SkipSpace(&p[c], &p[size]); // 空白やコントロールコードをスキップする
//			_objectParam.push_back(std::make_tuple(modelName, VGet(x, y, z), IsBreaking));
//		}
//	}
//	else {
//		return false;
//	}
//	return true;
//};
//
//
//bool ModeTest::Process() {
//	base::Process();
//
//	global._timer->TimeElapsed();
//	_sVib->UpdateScreenVibration();
//
//	_player->Process(_camera->GetCamY());
//
//	for (int i = 0; i < sizeof(ui) / sizeof(ui[0]); i++) {
//		ui[i]->Process();
//	}
//
//	_boss->Process();
//
//	bool isAttackState = _player->GetEnabledIBAttackCollision();
//	bool isInvincible = _player->GetIsInvincible();
//	VECTOR pPos = _player->GetPosition();
//
//
//	Sphere ibSphere = _player->GetIBCollision();
//	int ibPower = _player->GetPower();
//	Capsule plCol = _player->GetCollision();
//	Sphere ibCol = _player->GetIBCollision();
//	Capsule bossStakeCol = _boss->GetStakeCollision();
//
//	if (Collision3D::TwoCapsuleCol(plCol, bossStakeCol)) {
//		VECTOR vDir = VSub(plCol.down_pos, bossStakeCol.down_pos);
//		vDir.y = 0.0f;
//		vDir = VNorm(vDir);
//		float length = plCol.r + bossStakeCol.r;
//
//		VECTOR vPos = VAdd(bossStakeCol.down_pos, VScale(vDir, length));
//		_player->SetPos(vPos);
//	}
//
//
//	if (Collision3D::SphereCapsuleCol(ibCol, bossStakeCol)) {
//		VECTOR vDir = VSub(ibCol.centerPos, bossStakeCol.down_pos);
//		vDir.y = 0.0f;
//		vDir = VNorm(vDir);
//		float length = ibCol.r + bossStakeCol.r;
//
//		VECTOR vPos = VAdd(bossStakeCol.down_pos, VScale(vDir, length));
//		vPos.y = ibCol.centerPos.y;
//		_player->SetIBPos(vPos);
//	}
//
//	//for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
//	//	(*itr)->Process();
//
//	//	if ((*itr)->GetUse()) {
//	//		VECTOR tPos = (*itr)->GetPos();
//	//		Sphere tSphere = (*itr)->GetBottomSphereCollision();
//	//		if ((*itr)->GetCanBlast()) {
//	//			if (isAttackState) {
//	//				if (Collision3D::SphereCol(ibSphere, tSphere)) {
//
//	//					VECTOR vDir = VSub(tPos, pPos);
//	//					(*itr)->SetBlast(vDir);
//	//				}
//	//			}
//	//		}
//
//	//		// エネミーの押出処理
//	//		for (int i = 0; i < _enemyPool->ENEMY_MAX_SIZE; i++) {
//	//			EnemyBase* en = _enemyPool->GetEnemy(i);
//	//			if (!en) { continue; }
//	//			if (!en->GetUse()) { continue; }
//
//	//			float tR = tSphere.r;
//	//			VECTOR enPos = en->GetCollisionPos();
//	//			float enR = en->GetR();
//
//	//			enPos.y = 0;
//	//			tSphere.centerPos.y = 0;
//
//	//			VECTOR vDir = VSub(enPos, tSphere.centerPos);
//	//			if (VSize(vDir) <= enR + tR) {
//	//				float len = (enR + tR) - VSize(vDir);
//	//				vDir = VNorm(vDir);
//	//				en->SetPos(VAdd(enPos, VScale(vDir, len)));
//	//			}
//	//			en = nullptr;
//	//		}
//
//	//		// プレイヤーの押出
//	//		VECTOR pColPos = _player->GetPosition(); pColPos.y = 0;
//	//		float pR = _player->GetCollision().r;
//	//		float tR = tSphere.r;
//
//
//	//		VECTOR vDir = VSub(pColPos, tSphere.centerPos);
//	//		if (VSize(vDir) <= pR + tR) {
//	//			float len = (pR + tR) - VSize(vDir);
//	//			vDir = VNorm(vDir);
//	//			_player->SetPos(VAdd(pColPos, VScale(vDir, len)));
//	//		}
//	//	}
//	//}
//
//
//
//	if (XInput::GetInstance()->GetTrg(XINPUT_BUTTON_START)) {
//		ModeServer::GetInstance()->Add(NEW ModePause(), 10, "Pause");
//	}
//
//
//
//	if (XInput::GetInstance()->GetTrg(XINPUT_BUTTON_BACK)) {
//		_drawDebug = !_drawDebug;
//	}
//
//	if (_player->GetHP() <= 0) {
//		global._soundServer->BgmFadeOut(2000);
//		ModeServer::GetInstance()->Del(this);
//		ModeServer::GetInstance()->Add(NEW ModeGameOver(), 1, "gameover");
//	}
//
//	VECTOR box_vec = ConvWorldPosToScreenPos(VAdd(_player->GetPosition(), VGet(0, 170, 0)));
//
//	float ratio = 1.0f - _camera->GetTargetDistance() / _camera->GetMaxLength();
//	_gaugeUI[0]->Process(box_vec, _player->GetStamina(), _player->GetStaminaMax(), ratio);
//	_gaugeUI[1]->Process(box_vec, 100, 100, ratio);
//
//	_player->AnimationProcess();
//
//	TowerParts::CheckFinishedBlastTowerParts();
//
//	_effectManeger->Update();
//	_camera->Process(_player->GetPosition(), _tile);
//	return true;
//}
//
//
//bool ModeTest::GateProcess() {
//	return true;
//};// ゴールゲートの処理
//
//
//bool ModeTest::Render() {
//	SetUseZBuffer3D(TRUE);
//	SetWriteZBuffer3D(TRUE);
//	SetUseBackCulling(TRUE);
//
//	MV1DrawModel(_skySphere);
//
//	// 描画に使用するシャドウマップを設定
//	SetUseShadowMap(0, _shadowHandle);
//	MV1DrawModel(_tile);
//	// 描画に使用するシャドウマップの設定を解除
//	SetUseShadowMap(0, -1);
//
//
//	// ライト設定
//	SetUseLighting(TRUE);
//
//	// 0,0,0を中心に線を引く
//	{
//		float linelength = 1000.f;
//		VECTOR v = { 0, 0, 0 };
//		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
//		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
//		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
//	}
//
//	//------------------------------------
//	// シャドウマップの設定　
//	// shadowCount 0 シャドウマップに描画 1 モデルの描画
//	VECTOR lightDir = VGet(-0.5f, -1.0f, -0.5f);
//	SetShadowMapLightDirection(_shadowHandle, lightDir);
//
//	// シャドウマップに描画する範囲を設定
//	// カメラの注視点を中心にする
//	float length = 10000.f;
//	VECTOR plPos = _player->GetPosition();
//	SetShadowMapDrawArea(_shadowHandle, VAdd(plPos, VGet(-length, -1.0f, -length)), VAdd(plPos, VGet(length, length, length)));
//
//	for (int shadowCount = 0; shadowCount < 2; shadowCount++) {
//		// シャドウマップの設定
//		if (shadowCount == 0) {
//			// シャドウマップへの描画の準備
//			ShadowMap_DrawSetup(_shadowHandle);
//		}
//		else if (shadowCount == 1) {
//			// シャドウマップへの描画を終了
//			ShadowMap_DrawEnd();
//
//		}
//
//		//-------------------------------------------------------------------------------------
//
//		_player->Render();
//
//
//		_boss->Render();
//	}
//
//
//	if (_drawDebug) {
//		_player->DrawDebugInfo();
//		_boss->DrawDebugInfo();
//	}
//
//
//	SetUseZBuffer3D(FALSE);
//
//	_effectManeger->Render();
//
//
//	//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
//	for (int i = 0; i < sizeof(ui) / sizeof(ui[0]); i++) {
//		ui[i]->Draw();
//	}
//
//	if (_player->GetStaminaRate() < 1.0f) {
//		int handleNum = floorf(_player->GetStaminaRate() * 100.0f / 33.4f);
//		_gaugeUI[1]->Draw(_gaugeHandle[handleNum]);
//		_gaugeUI[0]->Draw(_gaugeHandle[3]);
//	}
//
//	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
//
//	SetFontSize(62);
//	DrawFormatString(45, 200, GetColor(0, 0, 0), "%d", _player->GetInstance()->GetNowLevel() + 1);
//	SetFontSize(16);
//	//for (auto itr = _buildingBase.begin(); itr != _buildingBase.end(); ++itr) {
//	//	(*itr)->DrawDebugInfo();
//	//}
//
//	return true;
//}
