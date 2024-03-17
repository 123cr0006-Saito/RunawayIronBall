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

	// デバッグ情報の表示
	void DrawDebugInfo();


private:
	void CheckState();

	void ResetPhase();


	// 待機状態の処理
	void IdleProcess();

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
	void RotationAcceleration();

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
	} _ibState;

	// 各ステート内でのフェーズ番号
	int _phase;
	int _phaseCnt;

	VECTOR _posBeforeMoving;
	VECTOR _targetPos;

	bool _isHitStake;

	//
	int _ibIdleCnt;
	VECTOR _ibMoveDir;

	int _ibStiffenCnt;



	// 
	bool _activeRotationAcceleration;
	int _rotationAccelerationCnt;
	float _rotAngularVelocity;
	float _rotAngle;

	// 鎖のモデルハンドル
	int _chainModelHandle;
	// 鎖の座標
	std::vector<VECTOR> _chainPos;

	float _chainDistance;

	// 杭の座標
	VECTOR* _stakePos;

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