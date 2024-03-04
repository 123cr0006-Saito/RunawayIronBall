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
		IB_STATE_ROTATION,
	} _ibState;


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
	// 一周にかかるまでのフレーム数
	float debugRotFrame = 60.0f;
};