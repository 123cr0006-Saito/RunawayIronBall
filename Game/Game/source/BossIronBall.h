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
	// 鉄球のモデルハンドル
	int _ibModelHandle;
	// 鉄球の座標
	VECTOR _ibPos;
	// 鉄球の当たり判定
	Sphere _ibSphereCol;

	// 鎖のモデルハンドル
	int _chainModelHandle;
	// 鎖の座標
	std::vector<VECTOR> _chainPos;

	float _chainDistance;

	// 杭の座標
	VECTOR* _stakePos;
};