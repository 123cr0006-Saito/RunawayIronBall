#pragma once
#include "appframe.h"
#include "BossIronBall.h"
#include "Player.h"

class Boss
{
public:
	Boss();
	~Boss();

	void LoadModel();
	void Init(VECTOR polePos);

	void Process();
	void Render();


	Capsule GetStakeCollision() { return _stakeCapsuleCol; }
	Sphere GetIBCollision() { return _ironBall->GetIBCollision(); }


	void CheckHitBossAndStake();

	void SetKnockBack(VECTOR vDir = VGet(0.0f, 0.0f, -1.0f)) { _ironBall->SetKnockBack(vDir); }

	// デバッグ情報の表示
	void DrawDebugInfo();

private:
	// 杭のモデルハンドル
	int _stakeModelHandle;
	// 杭の座標
	VECTOR _stakePos;
	// 杭の当たり判定
	Capsule _stakeCapsuleCol;

	BossIronBall* _ironBall;


	Player* _player;
};