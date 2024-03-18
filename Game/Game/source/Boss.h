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
	// �Y��HP�����炷
	void SetDamageStake(int damage) { _stakeHp -= damage; }
	// �Y��HP���擾
	int GetStakeHp() { return _stakeHp; }

	// �f�o�b�O���̕\��
	void DrawDebugInfo();

private:
	// �Y�̃��f���n���h��
	int _stakeModelHandle;
	// �Y�̍��W
	VECTOR _stakePos;
	// �Y�̓����蔻��
	Capsule _stakeCapsuleCol;
	// �Y��HP
	int _stakeHp;


	BossIronBall* _ironBall;


	Player* _player;
};