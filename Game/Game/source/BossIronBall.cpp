//----------------------------------------------------------------------
// @filename BossIronBall.cpp
// @date: 2024/03/20
// @author: Morozumi Hiroya
// @explanation
// ボス鉄球の制御を行うクラス
// ボス鉄球の制御は、BossIronBallクラスで行う
//----------------------------------------------------------------------
#include "BossIronBall.h"
#include "BossIronBall.h"

namespace {
	constexpr int BOSS_CHAIN_MAX = 12;
	constexpr float BOSS_CHAIN_TOTAL_LENGTH = 3000.0f;


	constexpr float SEARCH_RANGE[2] = { 2000.0f, 4000.0f };

	// --------- 

	// 待機状態
	// ジャンプ移動を行うフレーム数
	constexpr int IDLE_CNT_MAX = 30;
	// 1フレームあたりの移動量
	constexpr float IDLE_MOVE_SPEED = 180.0f / IDLE_CNT_MAX; // 30フレームで1.8m移動
	// 小ジャンプ後に次の行動に移るまでのフレーム数（120 ~ 180）
	// 基準値 
	constexpr int IDLE_INTERVAL_BASE = 120;
	// 乱数で追加されるフレーム数の最大値
	constexpr int IDLE_INTERVAL_ADD_MAX = 60;

	// 突進攻撃
	// 杭の付近に到達するまでのフレーム数
	constexpr int RU_REACH_STAKE_CNT = 60;
	// 溜め時間
	constexpr int RU_CHARGE_CNT = 60;
	// 突進を行うフレーム数
	constexpr int RU_ATTACK_CNT = 30;
	// 突進での移動距離
	constexpr float RU_MOVE_DISTANCE = 3700.0f;
	// 突進後の硬直時間（通常時）
	constexpr int RU_STIFFEN_CNT = 180;
	// 突進後の硬直時間（強化時）
	constexpr int RU_STIFFEN_CNT_ENHANCED = 90;

	// 落下攻撃
	// 飛び上がってから、最高地点に到達するまでのフレーム数
	constexpr int DR_REACH_HIGHEST_CNT = 60;
	// 最高地点に到達してから、地面に着地するまでのフレーム数
	constexpr int DR_REACH_GROUND_CNT = 20;
	// 地面に着地後の硬直時間（通常時）
	constexpr int DR_STIFFEN_CNT = 90;
	// 地面に着地後の硬直時間（強化状態時）
	constexpr int DR_STIFFEN_CNT_ENHANCED = 60;

	// 回転攻撃
	// 回転開始前に杭の付近に到達するまでのフレーム数
	constexpr int RO_REACH_STAKE_CNT = 60;
	// 回転方向
	constexpr float RO_ROTATION_DIR = -1.0f; // 反時計回り
	// 1フレームあたりの回転角	最小値
	constexpr float RO_ANGULAR_VELOCITY_MIN = (2.0f * DX_PI) / 90.0f; // 90フレームで一回転
	// 1フレームあたりの回転角	最大値
	constexpr float RO_ANGULAR_VELOCITY_MAX = (2.0f * DX_PI) / 20.0f; // 20フレームで一回転
	// 回転半径の最小値
	constexpr float RO_ROTAION_RADIUS_MIN = 500.0f;
	// 回転半径の最大値
	constexpr float RO_ROTAION_RADIUS_MAX = 3100.0f;

	// 最大速度に到達するまでのフレーム数
	constexpr int RO_ACCELERATION_CNT_MAX = 600;
	// 最大速度を維持するフレーム数
	constexpr int RO_MAINTAIN_MAXSPEED_CNT = 210;
	// 最大速度から、速度0まで減速しきるまでのフレーム数
	constexpr int RO_DECELERATION_CNT_MAX = 90;
	// 回転攻撃後の硬直時間
	constexpr int RO_STIFFEN_CNT = 300;

	// ノックバック
	// ガラス状態でのノックバック速度倍率
	constexpr float KB_SPEED_RATE_GLASS = 20.0f;

	// ハードノックバック
	// 杭に到達するまでのフレーム数
	constexpr int HK_REACH_STAKE_CNT = 60;
	// 杭にあたり跳ね返りを行うフレーム数
	constexpr int HK_BOUNCE_CNT = 45;
	// 跳ね返り距離
	constexpr float HK_BOUNCE_DISTANCE = 2000.0f;
	// 跳ね返り後の硬直時間
	constexpr int HK_STIFFEN_CNT = 60;

	// ---------

	// モデルの拡大率
	constexpr float MODEL_SCALE = 12.8f;
	// 鉄球部分の当たり判定の半径
	constexpr float IB_SPHERE_RADIUS = 20.0f * MODEL_SCALE;
	// 鎖部分の当たり判定の半径
	constexpr float CH_CAPSULE_RADIUS = 100.0f;
}

BossIronBall::BossIronBall()
{
	_ibModelHandle = -1;
	for (int i = 0; i < 2; i++) {
		_ibModelHandleArray[i] = -1;
	}
	_isGlass = false;

	_ibPos = VGet(0.0f, 0.0f, 0.0f);
	_ibModelForwardDir = VGet(0.0f, 0.0f, -1.0f);
	_ibModelNextForwardDir = VGet(0.0f, 0.0f, -1.0f);
	_ibModelDirState = IB_MODEL_DIR::PLAYER;

	_ibSphereCol.centerPos = _ibPos;
	_ibSphereCol.r = 0.0f;

	_chainCapsuleCol.down_pos = VGet(0.0f, 0.0f, 0.0f);
	_chainCapsuleCol.up_pos = VGet(0.0f, 0.0f, 0.0f);
	_chainCapsuleCol.r = 0.0f;
	_isRotationAttack = false;

	_isOnStage = true;

	_isInvincible = false;
	_useCollision = true;
	_isEnhanced = false;
	_changeEnhanced = false;
	_enhancedAttackCnt = 0;

	_ibState = IB_STATE::IDLE;

	_phase = 0;
	_phaseCnt = 0;

	_posBeforeMoving = VGet(0.0f, 0.0f, 0.0f);
	_targetPos = VGet(0.0f, 0.0f, 0.0f);

	_isHitStake = false;

	_ibIdleCnt = 0;
	_ibMoveDir = VGet(0.0f, 0.0f, 0.0f);


	_ibStiffenCnt = 0;

	_rotBaseDir = VGet(0.0f, 0.0f, 0.0f);
	_rotAngularVelocity = 0.0f;
	_rotAngle = 0.0f;
	_rotRadius = 0.0f;

	_isKnockBack = false;
	_knockBackDir = VGet(0.0f, 0.0f, -1.0f);
	_knockBackSpeed = 0.0f;
	_knockBackCnt = 0;
	_gravity = 0.0f;

	_chainModelHandle = -1;
	_chainPos.clear();
	_chainDistance = 0.0f;

	_stakePos = nullptr;
	_isStakeBroken = false;
	_mStakePos = MGetIdent();
	_mStakePosInv = MGetIdent();


	_animIndex = 0;
	_animTotalTime = 0.0f;
	_playTime = 0.0f;

	_playSound = false;

	_stageRadius = 0.0f;

	_player = nullptr;
}

BossIronBall::~BossIronBall()
{
	_stakePos = nullptr;
	_player = nullptr;
}

// モデルの読み込み
void BossIronBall::LoadModel()
{
	// 通常時の鉄球モデル
	_ibModelHandleArray[0] = ResourceServer::MV1LoadModel("Bossnake", "res/Enemy/Cg_Enemy_Bossnake/Cg_Enemy_Bossnake.mv1");
	// ガラス状態の鉄球モデル
	_ibModelHandleArray[1] = ResourceServer::MV1LoadModel("BossnakeEnhanced", "res/Enemy/Cg_Enemy_Bossnake/Bossnake_Glass.mv1");
	// 鎖のモデル
	_chainModelHandle = ResourceServer::MV1LoadModel("Chain", "res/Chain/Cg_Chain.mv1");
}

// 初期化処理
void BossIronBall::Init(VECTOR* stakePos)
{
	// 杭の座標を設定
	_stakePos = stakePos;

	// 鎖同士の間隔を設定
	_chainDistance = BOSS_CHAIN_TOTAL_LENGTH / BOSS_CHAIN_MAX;
	// 鎖の座標を設定
	for (int i = 0; i < BOSS_CHAIN_MAX; i++) {
		_chainPos.push_back(VAdd(*_stakePos, VGet(0.0f, 100.0f, -80.0f * i)));
	}

	// 鉄球の当たり判定を設定
	_ibSphereCol.centerPos = _ibPos;
	_ibSphereCol.r = IB_SPHERE_RADIUS;
	_chainCapsuleCol.down_pos = *_stakePos;
	_chainCapsuleCol.up_pos = _ibPos;
	_chainCapsuleCol.r = CH_CAPSULE_RADIUS;

	//	鉄球の初期座標を設定
	_ibPos = VGet(0, 0, -1000);
	_ibPos.y = _ibSphereCol.r;

	// 鉄球のモデルの拡大率を設定
	for (int i = 0; i < 2; i++) {
		MV1SetScale(_ibModelHandleArray[i], VScale(VGet(1.0f, 1.0f, 1.0f), MODEL_SCALE));
	}
	// 現在の鉄球モデルを設定
	_ibModelHandle = _ibModelHandleArray[0];

	// 行動状態を初期化
	_ibState = IB_STATE::IDLE;

	// 原点から杭の座標への平行移動行列を取得
	_mStakePos = MGetTranslate(*_stakePos);
	// _mStakePosの逆行列を取得
	_mStakePosInv = MInverse(_mStakePos);


	_rotAngle = 0.0f;

	//--------------------------------
	// アニメーション情報の初期化
	_animIndex = MV1AttachAnim(_ibModelHandle, 0, -1, FALSE);
	_animTotalTime = MV1GetAttachAnimTotalTime(_ibModelHandle, _animIndex);
	_playTime = 0.0f;
	//--------------------------------

	// プレイヤーのインスタンスを取得
	_player = Player::GetInstance();
}

// 更新処理
void BossIronBall::Process()
{
	// ステージ上にいる場合の処理
	if (_isOnStage) {

		// 行動状態に応じた処理を行う
		switch (_ibState)
		{
		// 待機
		case BossIronBall::IB_STATE::IDLE:
			IdleProcess();
			CheckChangeEnhanced();
			break;
		// 硬直
		case BossIronBall::IB_STATE::STIFFEN:
			StiffenProcess();
			CheckChangeEnhanced();
			break;
		// 突進攻撃
		case BossIronBall::IB_STATE::ATTACK_RUSH:
			RushProcess();
			break;
		// 落下攻撃
		case BossIronBall::IB_STATE::ATTACK_DROP:
			DropProcess();
			break;
		// 回転攻撃
		case BossIronBall::IB_STATE::ATTACK_ROTATION:
			RotationProcess();
			break;
		// ノックバック
		case BossIronBall::IB_STATE::KNOCK_BACK:
			KnockBackProcess();
			break;
		// ハードノックバック
		case BossIronBall::IB_STATE::HARD_KNOCK_BACK:
			HardKnockBackProcess();
			break;
		}


		_ibPos.y -= 16.0f;
		if (_ibPos.y - _ibSphereCol.r < 0.0f) _ibPos.y = _ibSphereCol.r;
	}
	else {
		_ibPos = VAdd(_ibPos, VScale(_knockBackDir, 30.0f));
		GravityProcess();
	}

	// 鉄球の当たり判定を更新
	UpdateIBCollision();

	// 回転攻撃中の場合、鎖の当たり判定を更新
	if (_isRotationAttack) {
		UpdateChainCollision();
		global._soundServer->DirectPlay("SE_BOSS_Rotate");
	}

	// 鉄球モデルの向きを更新
	UpdateModelRotation();

	// アニメーションの更新処理
	AnimationProcess();

	_isHitStake = false;

	// 鎖の更新処理
	ChainProcess();
}

// 描画処理
void BossIronBall::Render()
{
	// 鎖の描画
	for (int i = 0; i < BOSS_CHAIN_MAX; i++) {
		MV1SetPosition(_chainModelHandle, _chainPos[i]);
		MV1DrawModel(_chainModelHandle);
	}

	// 鉄球の描画
	MV1SetPosition(_ibModelHandle, _ibPos);
	MV1DrawModel(_ibModelHandle);
}

// ガラス状態にする
void BossIronBall::ChangeGlass()
{
	if (!_isGlass) {
		// ガラス状態の鉄球モデルに変更
		_ibModelHandle = _ibModelHandleArray[1];

		// アニメーション情報の設定
		_animIndex = MV1AttachAnim(_ibModelHandle, 0, -1, FALSE);
		_animTotalTime = MV1GetAttachAnimTotalTime(_ibModelHandle, _animIndex);
		_playTime = 0.0f;

		// SE再生
		global._soundServer->DirectPlay("SE_BOSS_Glass");

		// ガラス状態に設定
		_isGlass = true;
	}
}

// 鉄球の当たり判定を更新
void BossIronBall::UpdateIBCollision()
{
	_ibSphereCol.centerPos = _ibPos;
}

// 鎖の当たり判定を更新
void BossIronBall::UpdateChainCollision()
{
	_chainCapsuleCol.down_pos = *_stakePos;
	_chainCapsuleCol.down_pos.y = _chainCapsuleCol.r;
	_chainCapsuleCol.up_pos = _ibPos;
	_chainCapsuleCol.up_pos.y = _chainCapsuleCol.r;
}

// ステージ上にいるかどうかを設定する
void BossIronBall::SetOnStage(bool isOnStage)
{
	bool oldState = _isOnStage;
	_isOnStage = isOnStage;
	if (!oldState && isOnStage) {
		_gravity = 0.0f;
	}
}

// 鉄球モデルの向きを更新する
void BossIronBall::UpdateModelRotation()
{
	bool isUpdate = false;
	// IB_MODEL_DIRに応じた向きを設定
	switch (_ibModelDirState)
	{
	// プレイヤーの方向
	case IB_MODEL_DIR::PLAYER:
		_ibModelNextForwardDir = VSub(_player->GetPosition(), _ibPos);
		isUpdate = true;
		break;
	// プレイヤーの逆方向
	case IB_MODEL_DIR::PLAYER_REVERSE:
		_ibModelNextForwardDir = VSub(_ibPos, _player->GetPosition());
		isUpdate = true;
		break;
	// 杭の方向
	case IB_MODEL_DIR::STAKE:
		_ibModelNextForwardDir = VSub(*_stakePos, _ibPos);
		isUpdate = true;
		break;
	// 杭の逆方向
	case IB_MODEL_DIR::STAKE_REVERSE:
		_ibModelNextForwardDir = VSub(_ibPos, *_stakePos);
		isUpdate = true;
		break;
	// _ibModelNextForwardDirを更新しない（最後に設定された値を保持する）
	case IB_MODEL_DIR::NOT_UPDATE:
		//return;
		break;
	}

	// 向きを更新する場合
	if (isUpdate) {
		_ibModelNextForwardDir.y = 0.0f;
		_ibModelNextForwardDir = VNorm(_ibModelNextForwardDir);
	}

	// 鉄球モデルを滑らかに回転させる
	// モデルの現在の正面方向ベクトルと変更後の方向ベクトルの角度を計算する
	float angle = Math::CalcVectorAngle(_ibModelForwardDir, _ibModelNextForwardDir);
	// 1フレーム当たりの回転角
	float rotRad = (2.0f * DX_PI_F) / 20.0f;
	// angleがrotRadより小さい場合は_ibModelNextForwardDirをモデルの正面方向にする
	if (rotRad > angle) {
		_ibModelForwardDir = _ibModelNextForwardDir;
	}
	else {
		// rotRadの角度だけ回転させる
		VECTOR vN = VCross(_ibModelForwardDir, _ibModelNextForwardDir);
		_ibModelForwardDir = VTransform(_ibModelForwardDir, MGetRotAxis(vN, rotRad));
	}
	// モデルを指定の方向に向かせる
	Math::SetModelForward_RotationY(_ibModelHandle, _ibModelForwardDir);
}

// 行動状態を更新する
void BossIronBall::CheckState()
{
	// プレイヤーが鉄球の索敵範囲の内、どの範囲にいるかを取得
	int searchRangeIndex = -1;
	// 杭が破壊されていない場合
	if (!_isStakeBroken) {
		searchRangeIndex = CheckPlayerInSearchRange();
		if (searchRangeIndex == -1) {
			_ibState = IB_STATE::IDLE;
		}
		else if (searchRangeIndex == 0) {
			_ibState = IB_STATE::ATTACK_DROP;
		}
		else {
			_ibState = IB_STATE::ATTACK_RUSH;
		}
	}
	else {
		_ibState = IB_STATE::IDLE;
	}

	// 行動状態に応じた初期化処理を行う
	switch (_ibState)
	{
	// 待機
	case BossIronBall::IB_STATE::IDLE:
		SetIdle();
		break;
	// 硬直
	case BossIronBall::IB_STATE::STIFFEN:
		break;
	// 突進攻撃
	case BossIronBall::IB_STATE::ATTACK_RUSH:
		// 強化状態の場合の連続攻撃回数
		_enhancedAttackCnt = 3;
		SetRush();
		break;
	// 落下攻撃
	case BossIronBall::IB_STATE::ATTACK_DROP:
		// 強化状態の場合の連続攻撃回数
		_enhancedAttackCnt = 3;
		SetDrop();
		break;
	// 回転攻撃
	case BossIronBall::IB_STATE::ATTACK_ROTATION:
		SetRotation();
		break;
	}
}

// フェーズのリセット
// 各行動状態は複数のフェーズを持ち、行動状態が変わるとフェーズがリセットされる
void BossIronBall::ResetPhase()
{
	_phase = 0;
	_phaseCnt = 0;
}

// 待機状態の処理
void BossIronBall::IdleProcess()
{
	switch (_phase)
	{
	// フェーズ0: 移動
	case 0:
		_ibPos = VAdd(_ibPos, VScale(_ibMoveDir, IDLE_MOVE_SPEED));
		_ibPos.y = sinf(_phaseCnt / 30.0f * DX_PI) * 100.0f + _ibSphereCol.r;

		_phaseCnt++;

		// 移動フレーム数が一定数に達したら次のフェーズへ
		if (_phaseCnt > IDLE_CNT_MAX) {
			// 次のフェーズの設定

			// 待機時間を設定
			_ibIdleCnt = IDLE_INTERVAL_BASE + (rand() % IDLE_INTERVAL_ADD_MAX);

			// SE再生
			global._soundServer->DirectPlay("SE_Boss_Stay");

			_phaseCnt = 0;
			_phase++;
		}
		break;
	// フェーズ1: 待機
	case 1:
		_phaseCnt++;
		// 待機時間が経過したら次の行動状態へ
		if (_phaseCnt > _ibIdleCnt) {
			ResetPhase();
			CheckState();
		}
		break;
	}
}

// 待機状態に遷移する
void BossIronBall::SetIdle()
{
	ResetPhase();
	_isInvincible = false;

	_ibState = IB_STATE::IDLE;
	_ibIdleCnt = 0;

	// -100 ~ 100の範囲でランダムに移動方向を決定
	float x = (rand() % 201) - 100;
	float z = (rand() % 201) - 100;
	_ibMoveDir = VGet(x, 0.0f, z);
	_ibMoveDir = VNorm(_ibMoveDir);

	// 次にモデルを向ける方向を移動方向に設定
	_ibModelNextForwardDir = _ibMoveDir;
	// 上の行で設定した_ibModelNextForwardDirを維持するようにステートを設定
	_ibModelDirState = IB_MODEL_DIR::NOT_UPDATE;
}

// 硬直状態の処理
void BossIronBall::StiffenProcess()
{
	// SEを再生
	if (_playSound) {
		global._soundServer->DirectPlay("SE_BOSS_Confusion");
		_playSound = false;
	}

	_ibStiffenCnt--;
	// 硬直時間が経過したら待機状態に遷移
	if (_ibStiffenCnt < 0) {
		_ibStiffenCnt = 0;
		SetIdle();
	}
}

// 硬直状態に遷移する
// isInvincible: 無敵状態にするかどうか デフォルトはfalse
// playSound: SEを再生するかどうか デフォルトはtrue
void BossIronBall::SetStiffen(int cnt, bool isInvincible, bool playSound)
{
	_ibState = IB_STATE::STIFFEN;
	_ibStiffenCnt = cnt;
	_isInvincible = isInvincible;
	_playSound = playSound;
}

// 突進攻撃の処理
void BossIronBall::RushProcess()
{
	switch (_phase)
	{
	// フェーズ0: 杭の付近へ移動
	case 0:
		{
			// 移動処理
			// 水平方向は線形補間、鉛直方向はサイン波で移動
			VECTOR v = VGet(0.0f, 0.0f, 0.0f);
			v.x = Easing::Linear(_phaseCnt, _posBeforeMoving.x, _targetPos.x, RU_REACH_STAKE_CNT);
			v.y = _ibSphereCol.r + 500.0f * sinf(DX_PI_F * (_phaseCnt / static_cast<float>(RU_REACH_STAKE_CNT)));
			v.z = Easing::Linear(_phaseCnt, _posBeforeMoving.z, _targetPos.z, RU_REACH_STAKE_CNT);
			_ibPos = v;
		}

		// モデルの向きを杭の方向を向くように設定
		_ibModelDirState = IB_MODEL_DIR::STAKE;

		_phaseCnt++;
		// 杭の付近に到達したら次のフェーズへ
		if (_phaseCnt > RU_REACH_STAKE_CNT) {
			// 次のフェーズの設定

			// モデルの向きをプレイヤーの方向に設定
			VECTOR vDir = VSub(_player->GetPosition(), _ibPos);
			vDir.y = 0.0f;
			vDir = VNorm(vDir);
			_ibMoveDir = vDir;
			_ibModelDirState = IB_MODEL_DIR::PLAYER;

			// 移動前の座標を保存
			_posBeforeMoving = _ibPos;
			// 移動後の座標を設定
			// フェーズ0終了時にプレイヤーがいる方向を目標方向に設定する
			_targetPos = VScale(_ibMoveDir, RU_MOVE_DISTANCE);

			// SE再生
			global._soundServer->DirectPlay("SE_Boss_Stay");

			_phaseCnt = 0;
			_phase++;
		}
		break;
	// フェーズ1: 溜め
	case 1:
		// 前に設定した_ibModelNextForwardDirを更新しないしないように設定（フェーズ0終了時にプレイヤーがいた方向を向き続ける）
		_ibModelDirState = IB_MODEL_DIR::NOT_UPDATE;

		_phaseCnt++;
		// 溜め時間が経過したら次のフェーズへ
		if (_phaseCnt > RU_CHARGE_CNT) {
			// 次のフェーズの設定

			_playSound = true;

			_phaseCnt = 0;
			_phase++;
		}
		break;
	// フェーズ2: 突進
	case 2:
		{
			// フェーズ0終了時にプレイヤーがいる方向をが目標方向となる
			VECTOR v = VGet(0.0f, 0.0f, 0.0f);
			v.x = Easing::Linear(_phaseCnt, _posBeforeMoving.x, _targetPos.x, RU_ATTACK_CNT);
			v.y = _ibSphereCol.r;
			v.z = Easing::Linear(_phaseCnt, _posBeforeMoving.z, _targetPos.z, RU_ATTACK_CNT);
			_ibPos = v;
		}

		if (_playSound) {
			global._soundServer->DirectPlay("SE_BOSS_Rush");
			_playSound = false;
		}

		_phaseCnt++;
		// 突進が終了したら次のフェーズへ
		if (_phaseCnt > RU_ATTACK_CNT) {
			ResetPhase();

			// 強化状態の場合は連続攻撃
			if (_isEnhanced) {
				_enhancedAttackCnt--;
				// 連続攻撃回数が残っている場合は再度突進攻撃
				if (_enhancedAttackCnt > 0) {
					SetRush();
				}
				// 連続攻撃回数がない場合は硬直
				else {
					SetStiffen(RU_STIFFEN_CNT_ENHANCED);
				}
			}
			else {
				// 強化状態でない場合は硬直
				SetStiffen(RU_STIFFEN_CNT);
			}
		}
		break;
	}
}

// 突進攻撃に遷移する
void BossIronBall::SetRush()
{
	ResetPhase();
	_isInvincible = true;

	_ibState = IB_STATE::ATTACK_RUSH;

	// 移動前の座標を保存
	_posBeforeMoving = _ibPos;
	// 移動後の座標を設定（杭の付近に移動する）
	_targetPos = VGet(_stakePos->x, _ibSphereCol.r, _stakePos->z);
	VECTOR vDir = VSub(_posBeforeMoving, _targetPos);
	vDir.y = 0.0f;
	vDir = VNorm(vDir);
	_targetPos = VAdd(_targetPos, VScale(vDir, 500.0f));
}

// 落下攻撃の処理
void BossIronBall::DropProcess()
{
	switch (_phase)
	{
	// フェーズ0: 飛び上がり
	case 0:
	{
		// 飛び上がり処理
		// SetDrop()実行時に、プレイヤーがいた座標の上空に飛び上がる
		VECTOR highestPos = VAdd(_targetPos, VGet(0.0f, 500.0f, 0.0f));
		VECTOR v = VGet(0.0f, 0.0f, 0.0f);
		v.x = Easing::OutExpo(_phaseCnt, _posBeforeMoving.x, highestPos.x, DR_REACH_HIGHEST_CNT);
		v.y = Easing::OutExpo(_phaseCnt, _posBeforeMoving.y, highestPos.y, DR_REACH_HIGHEST_CNT);
		v.z = Easing::OutExpo(_phaseCnt, _posBeforeMoving.z, highestPos.z, DR_REACH_HIGHEST_CNT);
		_ibPos = v;

		// モデルの向きをプレイヤーの方向に設定
		_ibModelDirState = IB_MODEL_DIR::PLAYER;

		// SE再生
		if (_playSound) {
			global._soundServer->DirectPlay("SE_BOSS_Jump_Attack_01");
			_playSound = false;
		}

		_phaseCnt++;
		// 飛び上がりが終了したら次のフェーズへ
		if (_phaseCnt > DR_REACH_HIGHEST_CNT) {
			// 次のフェーズの設定

			_playSound = true;

			// 移動前の座標を保存（プレイヤーの上空）
			_posBeforeMoving = _ibPos;

			_phaseCnt = 0;
			_phase++;
		}
	}
	break;

	// フェーズ1: 着地
	case 1:
	{
		// 着地処理
		// SetDrop()実行時に、プレイヤーがいた座標に落下攻撃を行う
		VECTOR v = VGet(0.0f, 0.0f, 0.0f);
		v.x = Easing::EasingOutElastic(_phaseCnt, _posBeforeMoving.x, _targetPos.x, DR_REACH_GROUND_CNT);
		v.y = Easing::EasingOutElastic(_phaseCnt, _posBeforeMoving.y, _targetPos.y, DR_REACH_GROUND_CNT);
		v.z = Easing::EasingOutElastic(_phaseCnt, _posBeforeMoving.z, _targetPos.z, DR_REACH_GROUND_CNT);
		_ibPos = v;

		// 前に設定した_ibModelNextForwardDirを更新しないしないように設定 （フェーズ0終了時にプレイヤーがいた方向を向き続ける）
		_ibModelDirState = IB_MODEL_DIR::NOT_UPDATE;

		// SE再生
		if (_playSound) {
			if (_ibPos.y - _ibSphereCol.r < 0.0f) {
				global._soundServer->DirectPlay("SE_BOSS_Jump_Attack_02");
				_playSound = false;
			}
		}

		_phaseCnt++;
		// 着地が終了したら次のフェーズへ
		if (_phaseCnt > DR_REACH_GROUND_CNT) {

			ResetPhase();

			// 強化状態の場合は連続攻撃
			if (_isEnhanced) {
				_enhancedAttackCnt--;
				// 連続攻撃回数が残っている場合は再度落下攻撃
				if (_enhancedAttackCnt > 0) {
					SetDrop();
				}
				// 連続攻撃回数がない場合は硬直
				else { 
					SetStiffen(DR_STIFFEN_CNT_ENHANCED);
				}
			}
			else {
				// 強化状態でない場合は硬直
				SetStiffen(DR_STIFFEN_CNT);
			}
		}
	}
	break;
	}
}

// 落下攻撃に遷移する
void BossIronBall::SetDrop()
{
	ResetPhase();
	_isInvincible = true;

	_ibState = IB_STATE::ATTACK_DROP;

	// 移動前の座標を保存
	_posBeforeMoving = _ibPos;
	// 移動後の座標を設定（プレイヤーの上空に飛び上がる）
	_targetPos = _player->GetPosition();
	_targetPos.y = _ibSphereCol.r;

	_playSound = true;
}

// 回転攻撃の処理
void BossIronBall::RotationProcess()
{
	// フェーズ0以外（1~3）での回転処理
	if (_phase != 0) {
		// 杭を中心として回転する
		float angle = _rotAngle * RO_ROTATION_DIR;
		VECTOR v = VScale(_rotBaseDir, _rotRadius);
		v.y = _ibSphereCol.r;
		MATRIX mR = MGetRotY(angle);
		MATRIX m = MMult(_mStakePosInv, mR); // 杭の座標を中心に回転する
		m = MMult(m, _mStakePos);
		_ibPos = VTransform(v, m);

		// モデルの向きを設定
		MV1SetRotationXYZ(_ibModelHandle, VGet(0.0f, angle, 0.0f));
	}

	// 回転速度の更新処理
	switch (_phase)
	{
	// フェーズ0: 杭の付近まで戻る
	case 0:
		// 移動処理
		// 水平方向は線形補間、鉛直方向はサイン波で移動
		VECTOR v = VGet(0.0f, 0.0f, 0.0f);
		v.x = Easing::Linear(_phaseCnt, _posBeforeMoving.x, _targetPos.x, RO_REACH_STAKE_CNT);
		v.y = _ibSphereCol.r + 500.0f * sinf(DX_PI_F * (_phaseCnt / static_cast<float>(RO_REACH_STAKE_CNT)));
		v.z = Easing::Linear(_phaseCnt, _posBeforeMoving.z, _targetPos.z, RO_REACH_STAKE_CNT);
		_ibPos = v;

		// モデルの向きを杭の方向を向くように設定
		_ibModelDirState = IB_MODEL_DIR::STAKE;

		_phaseCnt++;
		// 杭の付近に到達したら次のフェーズへ
		if (_phaseCnt > RO_REACH_STAKE_CNT) {
			_rotRadius = RO_ROTAION_RADIUS_MIN;

			// モデルの向きを杭の逆方向に設定
			_ibModelDirState = IB_MODEL_DIR::STAKE_REVERSE;

			// SE再生
			global._soundServer->DirectPlay("SE_Boss_Stay");

			_phaseCnt = 0;
			_phase++;
		}
		break;
	// フェーズ1: 回転攻撃（加速）
	case 1:
		// 角速度を線形補間で更新
		_rotAngularVelocity = Easing::Linear(_phaseCnt, RO_ANGULAR_VELOCITY_MIN, RO_ANGULAR_VELOCITY_MAX, RO_ACCELERATION_CNT_MAX);
		// 回転半径を線形補間で更新
		_rotRadius = Easing::Linear(_phaseCnt, RO_ROTAION_RADIUS_MIN, RO_ROTAION_RADIUS_MAX, RO_ACCELERATION_CNT_MAX);

		_isRotationAttack = true;

		_phaseCnt++;
		// 加速が終了したら次のフェーズへ
		if (_phaseCnt > RO_ACCELERATION_CNT_MAX) {
			_phaseCnt = 0;
			_phase++;
		}
		break;
	// フェーズ2: 回転攻撃（最大速度維持）
	case 2:
		_phaseCnt++;
		// 一定時間経過したら次のフェーズへ
		if (_phaseCnt > RO_MAINTAIN_MAXSPEED_CNT) {
			_phaseCnt = 0;
			_phase++;
		}
		break;
	// フェーズ3: 回転攻撃（減速）
	case 3:
		// 角速度を線形補間で更新
		_rotAngularVelocity = Easing::Linear(_phaseCnt, RO_ANGULAR_VELOCITY_MAX, 0.0f, RO_DECELERATION_CNT_MAX);

		_phaseCnt++;
		// 減速が終了したら次のフェーズへ
		if (_phaseCnt > RO_DECELERATION_CNT_MAX) {
			_isRotationAttack = false;

			ResetPhase();
			// 硬直状態に遷移
			SetStiffen(RO_STIFFEN_CNT);
			return;
		}
		break;
	}

	// 次のフレームでの回転角度を更新
	_rotAngle += _rotAngularVelocity;
}

// 回転攻撃に遷移する
void BossIronBall::SetRotation()
{
	ResetPhase();
	_isInvincible = true;

	_ibState = IB_STATE::ATTACK_ROTATION;

	// 移動前の座標を保存
	_posBeforeMoving = _ibPos;

	// 回転の始点となる方向を設定
	_rotBaseDir = VSub(_ibPos, *_stakePos);
	_rotBaseDir.y = 0.0f;
	_rotBaseDir = VNorm(_rotBaseDir);

	// 移動後の座標を設定（杭の付近に移動する）
	_targetPos = VGet(_stakePos->x, _ibSphereCol.r, _stakePos->z);
	_targetPos = VAdd(_targetPos, VScale(_rotBaseDir, RO_ROTAION_RADIUS_MIN));

	_rotAngularVelocity = 0;
	_rotAngle = RO_ANGULAR_VELOCITY_MIN;
	_rotRadius = 0.0f;

	_playSound = true;
}

// 重力処理
void BossIronBall::GravityProcess()
{
	_ibPos.y += _gravity;
	if (_isOnStage &&  _ibPos.y - _ibSphereCol.r < 0.0f) {
		_gravity = 40.0f;
		global._soundServer->DirectPlay("SE_Boss_Stay");
	}
	else {
		_gravity -= 6.0f;
	}
}

// 鎖の更新処理
void BossIronBall::ChainProcess()
{
	_chainPos[BOSS_CHAIN_MAX - 1] = _ibPos;
	for (int i = 0; i < BOSS_CHAIN_MAX - 1; i++) {
		if (i == 0 && !_isStakeBroken) continue;
		_chainPos[i].y -= 16.0f;
		if (_chainPos[i].y - 20.0f < 0.0f) {
			//鉄球がステージ上にいるときは、地面からの押し出し処理を行う
			if (_isOnStage) {
				_chainPos[i].y = 20.0f;
			}
			// 鉄球がステージ上にいないときは、ステージの上にある鎖のみを押し出す
			else {
				VECTOR v = _chainPos[i];
				v.y = 0.0f;
				float squareLength = VSquareSize(v);
				squareLength -= powf(_ibSphereCol.r + 230.0f, 2);
				if (squareLength < powf(_stageRadius, 2)) {
					_chainPos[i].y = 20.0f;
				}
			}
		}
	}

	// 鎖同士の間隔を一定に保つ処理
	for (int i = BOSS_CHAIN_MAX - 1; i != 0; i--) {

		VECTOR vNext = _chainPos[i - 1];
		VECTOR vDelta = VSub(vNext, _chainPos[i]);
		float distance = VSize(vDelta);
		float difference = _chainDistance - distance;

		float offsetX = (difference * vDelta.x / distance) * 0.9f;
		float offsetY = (difference * vDelta.y / distance) * 0.9f;
		float offsetZ = (difference * vDelta.z / distance) * 0.9f;

		if (i != BOSS_CHAIN_MAX - 1) {
			_chainPos[i].x -= offsetX;
			_chainPos[i].y -= offsetY;
			_chainPos[i].z -= offsetZ;
		}

		if (i - 1 == 0 && !_isStakeBroken) continue;
		_chainPos[i - 1].x += offsetX;
		_chainPos[i - 1].y += offsetY;
		_chainPos[i - 1].z += offsetZ;
	}
}

// 強化状態に移行する
// 強化状態に遷移が可能なタイミングに呼び出し、初めて_changeEnhancedがtrueになったときに強化状態に移行する（この場合は_isEnhancedは必ずfalse）
void BossIronBall::CheckChangeEnhanced()
{
	if (_changeEnhanced && !_isEnhanced) {
		_isEnhanced = true;
		SetRotation();
	}
}

// プレイヤーが鉄球の索敵範囲内にいるかを取得する
// 索敵範囲内にいる場合は索敵範囲のインデックスを返す
// @return: -1: 探索範囲内にいない, 0: 探索範囲0にいる, 1: 探索範囲1にいる
int BossIronBall::CheckPlayerInSearchRange()
{
	int rangeIndex = -1;
	float squareDistance = VSquareSize(VSub(_player->GetPosition(), *_stakePos));
	if (squareDistance < SEARCH_RANGE[0] * SEARCH_RANGE[0]) {
		rangeIndex = 0;
	}
	else if (squareDistance < SEARCH_RANGE[1] * SEARCH_RANGE[1]) {
		rangeIndex = 1;
	}
	return rangeIndex;
}

// ノックバックの設定を行う
// @param vDir: ノックバック方向
// @param speed: ノックバック速度
void BossIronBall::SetKnockBack(VECTOR vDir, float speed)
{
	_isInvincible = true;
	_isKnockBack = true;

	// 硬直状態でない場合、かつ、杭が破壊されている場合はノックバック状態に遷移
	if (_ibState != IB_STATE::STIFFEN || _isStakeBroken) {
		_ibState = IB_STATE::KNOCK_BACK;

		_knockBackDir = VNorm(vDir);
		_knockBackSpeed = speed;

		// ガラス状態の場合はノックバック速度を増加
		if (_isGlass) _knockBackSpeed *= KB_SPEED_RATE_GLASS;

		_knockBackCnt = 30;
		_gravity = 80.0f;
	}
	else {
		// 硬直状態の場合はハードノックバック状態に遷移
		// （硬直時にプレイヤーの攻撃が当たった場合の状態, 杭まで直接飛んでいき、杭にダメージが入る）

		ResetPhase();
		_ibState = IB_STATE::HARD_KNOCK_BACK;

		// 移動前の座標を保存
		_posBeforeMoving = _ibPos;
		// 移動後の座標を設定（杭まで飛んでいく）
		_targetPos = *_stakePos;

		// 地面（y座標0）を基準とし、鉄球の半径の大きさの分だけy座標をプラスする
		_targetPos.y = _ibSphereCol.r;
	}
}

// ノックバック状態の処理
void BossIronBall::KnockBackProcess()
{
	if (_isKnockBack) {
		_ibPos = VAdd(_ibPos, VScale(_knockBackDir, _knockBackSpeed));
		GravityProcess();
		_knockBackCnt--;

		// ノックバックが終了したら硬直状態に遷移
		if (_knockBackCnt < 0) {
			_knockBackCnt = 0;
			_isKnockBack = false;

			// 硬直状態に遷移
			if (_isStakeBroken) {
				// 杭が破壊されている場合は硬直時に無敵化しない
				SetStiffen(30);
			}
			else {
				// 杭が破壊されていない場合は硬直時に無敵化を維持する
				SetStiffen(10, true, false);
			}
		}
	}
}

// ハードノックバック状態の処理
void BossIronBall::HardKnockBackProcess()
{
	switch (_phase)
	{
	// フェーズ0: 杭まで飛んでいく
	case 0:
		// 杭に当たったら次のフェーズへ
		if (_isHitStake) {
			// 次のフェーズの設定

			// 当たり判定を無効化
			_useCollision = false;

			// 移動前の座標を保存
			_posBeforeMoving = _ibPos;

			// 移動後の座標を設定（跳ね返り後の座標）
			VECTOR vDir = VGet(0.0f, 0.0f, -1.0f);
			// ランダムな方向へ跳ね返りを行う
			int angle = rand() % 360;
			vDir = VTransform(vDir, MGetRotY(angle));
			VECTOR vBase = *_stakePos;
			vBase.y = _ibSphereCol.r;
			_targetPos = VAdd(vBase, VScale(vDir, HK_BOUNCE_DISTANCE));

			_phaseCnt = 0;
			_phase++;
			break;
		}

		// 移動処理
		// 水平方向は線形補間、鉛直方向はサイン波で移動
		VECTOR v = VGet(0.0f, 0.0f, 0.0f);
		v.x = Easing::Linear(_phaseCnt, _posBeforeMoving.x, _targetPos.x, HK_REACH_STAKE_CNT);
		v.y = _ibSphereCol.r + 800.0f * sinf(DX_PI_F * (_phaseCnt / static_cast<float>(HK_REACH_STAKE_CNT)));
		v.z = Easing::Linear(_phaseCnt, _posBeforeMoving.z, _targetPos.z, HK_REACH_STAKE_CNT);
		_ibPos = v;

		_phaseCnt++;
		// 杭に到達したら次のフェーズへ
		if (_phaseCnt > HK_REACH_STAKE_CNT) {
			// 次のフェーズの設定

			// 当たり判定を無効化
			_useCollision = false;

			// 移動前の座標を保存
			_posBeforeMoving = _ibPos;

			// 移動後の座標を設定（跳ね返り後の座標）
			VECTOR vDir = VGet(0.0f, 0.0f, -1.0f);
			// ランダムな方向へ跳ね返りを行う
			int angle = rand() % 360;
			vDir = VTransform(vDir, MGetRotY(angle));
			VECTOR vBase = *_stakePos;
			vBase.y = _ibSphereCol.r;
			_targetPos = VAdd(vBase, VScale(vDir, HK_BOUNCE_DISTANCE));

			_phaseCnt = 0;
			_phase++;
		}
		break;
	// フェーズ1: 跳ね返り
	case 1:
		{
			// 移動処理
			// 水平方向は線形補間、鉛直方向はサイン波で移動
			VECTOR v = VGet(0.0f, _ibPos.y, 0.0f);
			v.x = Easing::Linear(_phaseCnt, _posBeforeMoving.x, _targetPos.x, HK_BOUNCE_CNT);
			v.y = _posBeforeMoving.y + 800.0f * sinf(DX_PI_F * (_phaseCnt / static_cast<float>(HK_BOUNCE_CNT)));
			v.z = Easing::Linear(_phaseCnt, _posBeforeMoving.z, _targetPos.z, HK_BOUNCE_CNT);
			_ibPos = v;
		}
		_phaseCnt++;
		// 跳ね返りが終了したら次のフェーズへ
		if (_phaseCnt > HK_BOUNCE_CNT) {
			// 次のフェーズの設定

			// 当たり判定を有効化
			_useCollision = true;

			_phaseCnt = 0;
			_phase++;
			CheckChangeEnhanced();
		}
		break;
	case 2: // 硬直
		_phaseCnt++;
		// 硬直時間が経過したら待機状態に遷移
		if (_phaseCnt > HK_STIFFEN_CNT) {
			_isKnockBack = false;
			ResetPhase();
			SetIdle();
		}
		break;
	}
}

// アニメーションの更新処理
void BossIronBall::AnimationProcess()
{
	MV1SetAttachAnimTime(_ibModelHandle, _animIndex, _playTime);

	_playTime += 1.0f;
	if (_animTotalTime < _playTime) {
		_playTime = 0.0f;
	}
}

// デバッグ情報の描画
void BossIronBall::DrawDebugInfo()
{
	auto color = _isInvincible ? COLOR_WHITE : COLOR_RED;
	_ibSphereCol.Render(color);
	color = _isRotationAttack ? COLOR_RED : COLOR_WHITE;
	_chainCapsuleCol.Render(color);

	for (int i = 0; i < 1; i++) {
		Sphere s = { *_stakePos, SEARCH_RANGE[i] };
		s.Render(COLOR_RED);
	}

	int x = 0;
	int y = 500;
	int line = 0;
	DrawBox(x, y, x + 300, y + 500, GetColor(0, 0, 0), TRUE);
	std::array<std::string, 7> stateStr = { "IDLE", "STIFFEN", "ATTACK_RUSH", "ATTACK_DROP", "ATTACK_ROTATION", "KnockBack", "HardKnockBack" };
	DrawFormatString(x, y + 20 * line, COLOR_WHITE, "State:%s", stateStr[static_cast<int>(_ibState)].c_str()); line++;
	DrawFormatString(x, y + 20 * line, COLOR_WHITE, "pos: x %3.2f, y %3.2f, z %3.2f", _ibPos.x, _ibPos.y, _ibPos.z); line++;
	DrawFormatString(x, y + 20 * line, COLOR_WHITE, "_phase: %d, _phaseCnt: %d", _phase, _phaseCnt); line++;

	//line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "idleCnt:%d", _ibIdleCnt); line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "idleCntMax:%d", debugFrameMax); line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "次に設定するフレーム数:%d", debugFrame); line++;
	//DrawFormatString(x, y + 20 * line, COLOR_WHITE, "次に設定する移動距離:%3.2f cm", debugValue); line++;
}