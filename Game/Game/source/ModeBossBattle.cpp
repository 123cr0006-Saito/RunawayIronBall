#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeBossBattle.h"
#include "ModeZoomCamera.h"
#include "ModeRotationCamera.h"

bool ModeBossBattle::Initialize() {
	if (!base::Initialize()) { return false; }

	_collisionManager = NEW CollisionManager();
	_collisionManager->Init();

	IsLoading = true;
	LoadFunctionThread = nullptr;

	_light = NEW Light("LightData");
	_timeLimit = NEW TimeLimit();
	_timeLimit->SetTimeLimit(12, 0);

	int resolution = 8192;
	_shadowHandle = MakeShadowMap(resolution, resolution);
	_skySphere = ResourceServer::MV1LoadModel("SkySpehe", "res/SkySphere/Skyspehre.mv1");
	_stage = ResourceServer::MV1LoadModel("BossStage", "res/BossStage/Stage_Boss.mv1");
	MV1SetPosition(_skySphere, VGet(0, 0, 0));
	MV1SetScale(_skySphere, VScale(VGet(1, 1, 1), 0.5f));
	MV1SetPosition(_stage, VGet(0, -5000.0f, 0));

	int playerModelHandle = ResourceServer::MV1LoadModel("Player", "res/Character/cg_player_girl/Cg_Player_Girl.mv1");
	_player = NEW Player();
	_player->Init(playerModelHandle, VGet(0, 0, -1000));
	_camera = NEW Camera(_player->GetPosition());



	_boss = NEW Boss();
	_boss->LoadModel();
	_boss->Init(VGet(0, 0, 0));

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
	}

	_suppression = NEW Suppression();

	_floor = NEW Floor();



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


	return true;
}

bool ModeBossBattle::Terminate() {
	base::Terminate();
	delete _collisionManager;
	delete _camera;
	delete _player;
	delete _boss;
	delete _sVib;
	delete _effectManeger;
	delete _classificationEffect;
	delete _light;
	delete _timeLimit;
	delete _floor;


	if (LoadFunctionThread != nullptr) {
		LoadFunctionThread->detach();
		delete LoadFunctionThread; LoadFunctionThread = nullptr;
	}

	for (int i = 0; i < 4; i++) {
		delete ui[i];
	}

	for (int i = 0; i < 2; i++) {
		delete _gaugeUI[i];
	}

	
	ResourceServer::MV1DeleteModel("SkySpehe", _skySphere);
	_skySphere = -1;
	ResourceServer::MV1DeleteModel("BossStage", _stage);
	_stage = -1;	

	return true;
}

bool ModeBossBattle::Process() {
	base::Process();
	global._timer->TimeElapsed();
	_sVib->UpdateScreenVibration();

	_player->Process(_camera->GetCamY());

	_boss->Process();

	_timeLimit->Process();



	Capsule pCol = _player->GetCollision();
	Capsule bSCol = _boss->GetStakeCollision();
	bSCol.down_pos.y = 0.0f;
	
	VECTOR vSub = VSub(pCol.down_pos, bSCol.down_pos);
	vSub.y = 0.0f;
	float squareLength = VSquareSize(vSub);
	if (squareLength < (bSCol.r + pCol.r) * (bSCol.r + pCol.r)) {
		VECTOR vDir = vSub;
		vDir = VNorm(vDir);
		float extrudeLength = bSCol.r + pCol.r;
		VECTOR vMove = VAdd(bSCol.down_pos, VScale(vDir, extrudeLength));
		_player->SetPos(vMove);
	}

	bool enabledIBAttackCollision = _player->GetEnabledIBAttackCollision();
	if (enabledIBAttackCollision) {

		Sphere pIBCol = _player->GetIBCollision();
		Sphere bIBCol = _boss->GetIBCollision();
		if (Collision3D::SphereCol(pIBCol, bIBCol)) {
			VECTOR vDir = VSub(bIBCol.centerPos, pCol.down_pos);
			vDir.y = 0.0f;
			vDir = VNorm(vDir);
			_boss->SetKnockBack(vDir);
		}

		if (Collision3D::SphereCapsuleCol(pIBCol, bSCol)) {
			_boss->SetDamageStake(3);
		}
	}



	//for (int i = 0; i < sizeof(ui) / sizeof(ui[0]); i++) {
	//	ui[i]->Process();
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
		ModeServer::GetInstance()->Del(this);
		//ModeServer::GetInstance()->Add(NEW ModeGameOver(), 1, "gameover");
	}

	VECTOR box_vec = ConvWorldPosToScreenPos(VAdd(_player->GetPosition(), VGet(0, 170, 0)));

	float ratio = 1.0f - _camera->GetTargetDistance() / _camera->GetMaxLength();
	_gaugeUI[0]->Process(box_vec, _player->GetStamina(), _player->GetStaminaMax(), ratio);
	_gaugeUI[1]->Process(box_vec, 100, 100, ratio);

	_collisionManager->Process();

	_player->AnimationProcess();


	_effectManeger->Update();
	_camera->Process(_player->GetPosition(), _stage);

	return true;
}

bool ModeBossBattle::Render() {
	if (LoadFunctionThread)return true;

	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	MV1DrawModel(_skySphere);

	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, _shadowHandle);
	MV1DrawModel(_stage);
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
		_boss->Render();
	}


	if (_drawDebug) {
		_player->DrawDebugInfo();
		_boss->DrawDebugInfo();
		_collisionManager->DrawAreaIndex();
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
	//for (auto itr = _buildingBase.begin(); itr != _buildingBase.end(); ++itr) {
	//	(*itr)->DrawDebugInfo();
	//}

	return true;
}
