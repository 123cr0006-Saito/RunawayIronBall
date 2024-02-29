#pragma once
#include "appframe.h"
#include "BossIronBall.h"

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
};