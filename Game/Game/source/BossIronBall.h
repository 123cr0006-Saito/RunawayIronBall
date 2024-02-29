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



	// デバッグ情報の表示
	void DrawDebugInfo();

private:
	// 鉄球のモデルハンドル
	int _ibModelHandle;
	// 鉄球の座標
	VECTOR _ibPos;

	// 鎖のモデルハンドル
	int _chainModelHandle;
	// 鎖の座標
	std::vector<VECTOR> _chainPos;

	float _chainDistance;

	// 杭の座標
	VECTOR* _stakePos;
};