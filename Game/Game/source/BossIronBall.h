#pragma once
#include "appframe.h"

class BossIronBall
{
public:
	BossIronBall();
	~BossIronBall();

	void LoadModel();
	void Init(VECTOR* stakePos);

	void Process();
	void Render();


	void UpdateIBCollision();

	// デバッグ情報の表示
	void DrawDebugInfo();


private:
	void CheckState();




	// 待機状態の処理
	void IdleProcess();

	// 硬直状態の処理
	void StiffenProcess();
	void SetStiffen(int cnt) { _ibState = IB_STATE::IB_STATE_STIFFEN; _ibStiffenCnt = cnt; }

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
		IB_STATE_IDLE,
		IB_STATE_STIFFEN,
		IB_STATE_ATTACK_RUSH,
		IB_STATE_ATTACK_DROP,
		IB_STATE_ATTACK_ROTATION,
	} _ibState;

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






	// デバッグ用
	// 特定のアクションのフレーム数
	int debugFrame = 30;
	int debugFrameMax = 30;

	float debugValue = 100.0f;
	float debugValueMax = 100.0f;
};