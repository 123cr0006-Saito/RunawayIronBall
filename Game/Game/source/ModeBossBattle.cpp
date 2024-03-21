#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeBossBattle.h"
#include "ModeZoomCamera.h"
#include "ModeRotationCamera.h"
#include "ModePause.h"
#include "ModeGameOver.h"
#include "ModeLoading.h"

namespace {
	// ステージの半径
	constexpr float STAGE_RADIUS = 8000.0f / 2.0f;
}

bool ModeBossBattle::Initialize() {
	if (!base::Initialize()) { return false; }

	_collisionManager = NEW CollisionManager();
	_collisionManager->Init();

	_light = NEW Light("LightData");

	int resolution = 8192;
	_shadowHandle = MakeShadowMap(resolution, resolution);
	_skySphere = ResourceServer::MV1LoadModel("SkySpehe", "res/SkySphere/Skyspehre.mv1");
	_stage = ResourceServer::MV1LoadModel("BossStage", "res/BossStage/Stage_Boss.mv1");
	MV1SetPosition(_skySphere, VGet(0, 0, 0));
	MV1SetScale(_skySphere, VScale(VGet(1, 1, 1), 0.5f));
	MV1SetPosition(_stage, VGet(0, -4995.0f, 0));

	int playerModelHandle = ResourceServer::MV1LoadModel("Player", "res/Character/cg_player_girl/Cg_Player_Girl.mv1");
	_player = NEW Player();
	_player->Init(playerModelHandle, VGet(0, 0, -1000));
	_player->SetLevel(global._allExp);
	_camera = NEW Camera(_player->GetPosition());

	_boss = NEW Boss();
	_boss->LoadModel();
	_boss->Init(VGet(0, 0, 0));
	_boss->SetStageRadius(STAGE_RADIUS);

	_classificationEffect = NEW ClassificationEffect();
	_effectManeger = NEW EffectManeger();


	{
		global._soundServer->Add("SE_Boss_Stay", new SoundItemSE("Sound/SE/Enemy/SE_Boss_Stay.wav"));
		global._soundServer->Add("SE_BOSS_Confusion", new SoundItemSE("Sound/SE/Enemy/SE_BOSS_Confusion.wav"));
		global._soundServer->Add("SE_BOSS_Rush", new SoundItemSE("Sound/SE/Enemy/SE_BOSS_Rush.wav"));
		global._soundServer->Add("SE_BOSS_Jump_Attack_01", new SoundItemSE("Sound/SE/Enemy/SE_BOSS_Jump_Attack_01.wav"));
		global._soundServer->Add("SE_BOSS_Jump_Attack_02", new SoundItemSE("Sound/SE/Enemy/SE_BOSS_Jump_Attack_02.wav"));
		global._soundServer->Add("SE_BOSS_Rotate", new SoundItemSE("Sound/SE/Enemy/SE_BOSS_Rotate.wav"));
		global._soundServer->Add("SE_BOSS_Glass", new SoundItemSE("Sound/SE/Enemy/SE_BOSS_Glass.wav"));
	}

	int size = 100;
	int heartHandle[3];
	ResourceServer::LoadMultGraph("Heart", "res/UI/Heart/UI_Heart", ".png", 3, heartHandle);
	ui[0] = NEW UIHeart(VGet(120, 20, 0), 3, heartHandle, 2);
	ui[1] = NEW UIExpPoint(VGet(100, 150, 0));
	_bossHp = NEW UIBossHp(VGet(1000,700,0));
	_gaugeUI[0] = NEW DrawGauge(0, 3, size, true);
	_gaugeUI[1] = NEW DrawGauge(0, 3, size, true);
	_gaugeHandle[0] = ResourceServer::LoadGraph("Stamina03", ("res/UI/Stamina/UI_Stamina_03.png"));
	_gaugeHandle[1] = ResourceServer::LoadGraph("Stamina02", ("res/UI/Stamina/UI_Stamina_02.png"));
	_gaugeHandle[2] = ResourceServer::LoadGraph("Stamina01", ("res/UI/Stamina/UI_Stamina_01.png"));
	_gaugeHandle[3] = ResourceServer::LoadGraph("Stamina04", ("res/UI/Stamina/UI_Stamina_04.png"));
	
	ModeServer::GetInstance()->Add(NEW ModeRotationCamera(4), 10, "RotCamera");
	return true;
}

bool ModeBossBattle::Terminate() {
	base::Terminate();
	delete _collisionManager;
	delete _camera;
	delete _player;
	delete _boss;
	delete _effectManeger;
	delete _classificationEffect;
	delete _light;
	delete _bossHp;
	for (int i = 0; i < 2; i++) {
		delete ui[i];
	}

	for (int i = 0; i < 2; i++) {
		delete _gaugeUI[i];
	}

	return true;
}

bool ModeBossBattle::Process() {
	base::Process();
	global._timer->TimeElapsed();

	if (XInput::GetInstance()->GetTrg(XINPUT_BUTTON_RIGHT_THUMB)) {
		_boss->SetDamageStake(50);
	}

	_player->Process(_camera->GetCamY());

	_boss->Process();

	_bossHp->Process(_boss->GetStakeHp(), _boss->GetStakeMaxHp());


	

	// 杭が破壊されていない場合に判定を行う
	if (_boss->GetIsStakeBroken() == false) {
		Capsule pCol = _player->GetCollision();
		Capsule bSCol = _boss->GetStakeCollision();
		bSCol.down_pos.y = 0.0f;

		// プレイヤーとボス杭の当たり判定
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
	}

	// プレイヤーからボスへの攻撃
	// プレイヤーの攻撃判定が有効なら
	if (_player->GetEnabledIBAttackCollision()) {
		Capsule pCol = _player->GetCollision();
		Sphere pIBCol = _player->GetIBCollision();

		// ボスが無敵状態でなければ
		if (_boss->GetIBInvincible() == false) {
			bool isHit = false;
			Sphere bIBCol = _boss->GetIBCollision();

			// プレイヤー鉄球とボス鉄球の当たり判定
			if (Collision3D::SphereCol(pIBCol, bIBCol)) {
				VECTOR vDir = VSub(bIBCol.centerPos, pCol.down_pos);
				vDir.y = 0.0f;
				_boss->SetIBKnockBack(vDir, 12.0f);
				if(_boss->GetIsGlass()){
					global._soundServer->DirectPlay("SE_BOSS_Glass");
				}
				else {
					global._soundServer->DirectPlay("House_Iron_Hit");
				}
				isHit = true;
			}

			// プレイヤー鎖とボス鉄球の当たり判定
			// プレイヤー鉄球が当たっている場合は、判定を行わない
			if (!isHit) {
				Capsule pCCol = _player->GetChainCollision();
				if (Collision3D::SphereCapsuleCol(bIBCol, pCCol)) {
					VECTOR vDir = VSub(bIBCol.centerPos, pCol.down_pos);
					vDir.y = 0.0f;
					_boss->SetIBKnockBack(vDir, 12.0f);
					if (_boss->GetIsGlass()) {
						global._soundServer->DirectPlay("SE_BOSS_Glass");
					}
					else {
						global._soundServer->DirectPlay("House_Iron_Hit");
					}
				}
			}
		}

		// 杭が破壊されていない場合に判定を行う
		if (_boss->GetIsStakeBroken() == false) {
			bool isHit = false;
			Capsule bSCol = _boss->GetStakeCollision();

			// プレイヤー鉄球とボス杭の当たり判定
			if (Collision3D::SphereCapsuleCol(pIBCol, bSCol)) {
				_boss->SetDamageStake(3);
				global._soundServer->DirectPlay("House_Iron_Hit");
				isHit = true;
			}

			// プレイヤー鎖とボス杭の当たり判定
			// プレイヤー鉄球が当たっている場合は、判定を行わない
			if (!isHit) {
				Capsule pCCol = _player->GetChainCollision();
				if (Collision3D::TwoCapsuleCol(pCCol, bSCol)) {
					_boss->SetDamageStake(3);
					global._soundServer->DirectPlay("House_Iron_Hit");
				}
			}
		}
	}

	// ボスからプレイヤーへの攻撃
	{
		bool isHit = false;
		Capsule pCol = _player->GetCollision();
		Sphere bIBCol = _boss->GetIBCollision();
		VECTOR bSPos = _boss->GetStakePosition();

		// ボス鉄球とプレイヤーの当たり判定
		if (Collision3D::SphereCapsuleCol(bIBCol, pCol)) {
			VECTOR vDir = VGet(0.0f, 0.0f, 0.0f);
			// 回転攻撃中でなければ、ボス鉄球から見たプレイヤーの方向にはじく
			if (_boss->GetIsRotationAttack() == false) {
				vDir = VSub(pCol.down_pos, bIBCol.centerPos);
			}
			// 回転攻撃中なら、ボス杭から見たプレイヤーの方向にはじく（ステージの外側にはじく）
			else {
				vDir = VSub(pCol.down_pos, bSPos);
			}
			// y成分を0にする
			vDir.y = 0.0f;			
			_player->SetBlastOffPower(vDir, 40.0f);
			// 無敵状態でなければダメージを与える
			if (_player->GetIsInvincible() == false) {
				_player->SetDamage();
			}
			isHit = true;
		}

		// ボス鎖とプレイヤーの当たり判定
		// ボス鉄球が当たっている場合は、判定を行わない
		if (!isHit) {
			// ボス鎖の当たり判定は回転攻撃中にのみ行う
			if (_boss->GetIsRotationAttack()) {
				Capsule bCCol = _boss->GetChainCollision();
				if (Collision3D::TwoCapsuleCol(bCCol, pCol)) {
					// ボス杭から見たプレイヤーの方向にはじく（ステージの外側にはじく）
					VECTOR vDir = VSub(pCol.down_pos, bSPos);
					vDir.y = 0.0f;
					_player->SetBlastOffPower(vDir, 40.0f);
					// 無敵状態でなければダメージを与える
					if (_player->GetIsInvincible() == false) {
						_player->SetDamage();
					}
				}
			}
		}
	}

	// プレイヤーの移動制限
	VECTOR pPos = _player->GetPosition();
	pPos.y = 0.0f;
	float squareLength = VSquareSize(pPos);
	float moveLength = STAGE_RADIUS - 170.0f;
	if (squareLength > powf(moveLength, 2)) {
		VECTOR vDir = VNorm(pPos);
		VECTOR vMove = VScale(vDir, moveLength);
		_player->SetPos(vMove);
	}


	// ボス鉄球がステージから出ているかを判定
	// 杭が破壊されている場合に判定をする
	if (_boss->GetIsStakeBroken()) {
		Sphere bIBCol = _boss->GetIBCollision();

		VECTOR vDir = VSub(bIBCol.centerPos, VGet(0, 0, 0));
		vDir.y = 0.0f;
		float squareLength = VSquareSize(vDir);
		squareLength -= powf(bIBCol.r + 20.0f, 2);

		if (_boss->GetOnStage()) {
			if (squareLength > powf(STAGE_RADIUS, 2)) {
				_boss->SetOnStage(false);
			}
		}
		else {
			if (squareLength < powf(STAGE_RADIUS, 2)) {
				VECTOR v = VGet(0, 0, 0);
				vDir = VNorm(vDir);
				_boss->SetIBPosition(VAdd(v, VScale(vDir, STAGE_RADIUS + bIBCol.r + 150.0f)));
			}
		}
	}
	VECTOR p = _boss->GetIBPosition();
	if(!ModeServer::GetInstance()->Search("Fade") && p.y < -5000) {
		ModeServer::GetInstance()->Add(NEW ModeFadeComeBack(2500, this,  50), 100, "Fade");
		ModeServer::GetInstance()->Add(NEW ModeScenario("Data/ScenarioData/Scenario03.csv", 3), 0, "Scenario");
	}

	for (int i = 0; i < sizeof(ui) / sizeof(ui[0]); i++) {
		ui[i]->Process();
	}


	if (XInput::GetInstance()->GetTrg(XINPUT_BUTTON_START)) {
		//_enemyPool->Init();
		//_player->SetDamage();
		ModeServer::GetInstance()->Add(NEW ModePause(), 10, "Pause");
	}


	if (XInput::GetInstance()->GetTrg(XINPUT_BUTTON_BACK)) {
		_drawDebug = !_drawDebug;
	}

	if (_player->GetHP() <= 0) {
		global._soundServer->BgmFadeOut(2000);
		ModeServer::GetInstance()->Add(NEW ModeGameOver(this), 0, "GameOver");
		ModeServer::GetInstance()->Add(NEW ModeFadeComeBack(2500, this,"GameOver", 50), 100, "Fade");
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

	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	MV1DrawModel(_skySphere);

	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, _shadowHandle);
	MV1DrawModel(_stage);
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

	if (!ModeServer::GetInstance()->Search("RotCamera")) {
	    for (int i = 0; i < sizeof(ui) / sizeof(ui[0]); i++) {
	    	ui[i]->Draw();
	    }
	    _bossHp->Draw();
	    
	    if (_player->GetStaminaRate() < 1.0f) {
	    	int handleNum = floorf(_player->GetStaminaRate() * 100.0f / 33.4f);
	    	_gaugeUI[1]->Draw(_gaugeHandle[handleNum]);
	    	_gaugeUI[0]->Draw(_gaugeHandle[3]);
	    }
	}

	DrawFormatString(1800, 1000, COLOR_RED, "StakeHP:%d", _boss->GetStakeHp());

	//for (auto itr = _buildingBase.begin(); itr != _buildingBase.end(); ++itr) {
	//	(*itr)->DrawDebugInfo();
	//}

	return true;
}
