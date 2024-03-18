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

	int SearchPlayer();

	Capsule GetStakeCollision() { return _stakeCapsuleCol; }
	Sphere GetIBCollision() { return _ironBall->GetIBCollision(); }


	void CheckHitBossAndStake();


	// �f�o�b�O���̕\��
	void DrawDebugInfo();

private:
	// �Y�̃��f���n���h��
	int _stakeModelHandle;
	// �Y�̍��W
	VECTOR _stakePos;
	// �Y�̓����蔻��
	Capsule _stakeCapsuleCol;

	std::array<float, 2> _searchRange;

	BossIronBall* _ironBall;


	Player* _player;
};