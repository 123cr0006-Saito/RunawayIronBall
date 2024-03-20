#pragma once
#include "appframe.h"
#include "Player.h"

class BossIronBall
{
public:
	BossIronBall();
	~BossIronBall();

	void LoadModel();
	void Init(VECTOR* stakePos);

	void Process();
	void Render();

	VECTOR GetPosition() { return _ibPos; }
	void SetPosition(VECTOR pos) { _ibPos = pos; }


	void UpdateIBCollision();
	Sphere GetIBCollision() { return _ibSphereCol; }

	void SetHitStake(bool isHit) { _isHitStake = isHit; }
	bool GetHitStake() { return _isHitStake; }

	void SetISStakeBroken(bool isBroken) { _isStakeBroken = isBroken; }


	int CheckPlayerInSearchRange();

	void SetKnockBack(VECTOR vDir = VGet(0.0f, 0.0f, -1.0f)) {
		_isKnockBack = true;
		_ibState = IB_STATE::KNOCK_BACK;
		_knockBackDir = VNorm(vDir);
		_knockBackCnt = 30;
		_gravity = 80.0f;
	}
	bool GetKnockBack() { return _isKnockBack; }

	// デバッグ情報の表示
	void DrawDebugInfo();


private:
	void CheckState();

	void ResetPhase();


	// 待機状態の処理
	void IdleProcess();
	void SetIdle();

	// 硬直状態の処理
	void StiffenProcess();
	void SetStiffen(int cnt) { _ibState = IB_STATE::STIFFEN; _ibStiffenCnt = cnt; }


	// 突進攻撃
	void RushProcess();
	void SetRush();

	// 落下攻撃
	void DropProcess();
	void SetDrop();

	// 回転攻撃
	void RotationProcess();
	void SetRotation();

	// ノックバック処理
	void KnockBackProcess();

	void ChainProcess();

private:
	// 鉄球のモデルハンドル
	int _ibModelHandle;
	// 鉄球の座標
	VECTOR _ibPos;
	// 鉄球の当たり判定
	Sphere _ibSphereCol;

	enum class IB_STATE {
		IDLE,							// 待機
		STIFFEN,					// 硬直
		ATTACK_RUSH,			// 突進攻撃
		ATTACK_DROP,			// 落下攻撃
		ATTACK_ROTATION,	// 回転攻撃
		KNOCK_BACK,			// ノックバック
	} _ibState;

	// 各ステート内でのフェーズ番号
	int _phase;
	int _phaseCnt;

	VECTOR _posBeforeMoving;
	VECTOR _targetPos;

	bool _isHitStake;
	bool _reachedStake;

	//
	int _ibIdleCnt;
	VECTOR _ibMoveDir;

	int _ibStiffenCnt;



	// 回転攻撃
	//  基準方向ベクトル
	VECTOR _rotBaseDir;
	// 角速度
	float _rotAngularVelocity;
	// _rotBaseDirを基準とした回転角度
	float _rotAngle;
	// 回転の半径
	float _rotRadius;


	// ノックバック状態かどうか
	bool _isKnockBack;
	// ノックバック方向
	VECTOR _knockBackDir;
	// ノックバックを行うの残りフレーム数
	int _knockBackCnt;

	float _gravity;

	// 鎖のモデルハンドル
	int _chainModelHandle;
	// 鎖の座標
	std::vector<VECTOR> _chainPos;

	float _chainDistance;

	// 杭の座標
	VECTOR* _stakePos;
	// 杭が破壊されたかどうか
	bool _isStakeBroken;

	// 原点から杭の座標への平行移動行列
	MATRIX _mStakePos;
	// _mStakePosの逆行列
	MATRIX _mStakePosInv;



	Player* _player;


	// デバッグ用
	// 特定のアクションのフレーム数
	int debugFrame = 30;
	int debugFrameMax = 30;

	float debugValue = 100.0f;
	float debugValueMax = 100.0f;
};