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






	// �f�o�b�O���̕\��
	void DrawDebugInfo();

private:
	// �Y�̃��f���n���h��
	int _stakeModelHandle;
	// �Y�̍��W
	VECTOR _stakePos;
	// �Y�̓����蔻��
	Capsule _stakeCapsuleCol;

	BossIronBall* _ironBall;
};