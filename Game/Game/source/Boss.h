#pragma once
#include "appframe.h"
#include "BossIronBall.h"
#include "Player.h"

namespace {
	// �Y�̍ő�HP
	constexpr int STAKE_MAX_HP = 100;
}
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
	bool GetIBInvincible() { return _ironBall->GetIsInvincible(); }

	void CheckHitBossAndStake();

	void SetIBKnockBack(VECTOR vDir, float speed) { _ironBall->SetKnockBack(vDir, speed); }
	// �Y��HP�����炷
	void SetDamageStake(int damage);

	// �Y��HP���擾
	int GetStakeHp() { return _stakeHp; }
	// �Y�̍ő�HP���擾
	int GetStakeMaxHp() { return STAKE_MAX_HP; }

	bool GetIsStakeBroken() { return _isStakeBroken; }

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
	// �Y�����G��Ԃ��ǂ���
	bool _isStakeInvincible;
	// �Y�̖��G����
	int _stakeInvincibleCnt;
	// �Y���j�󂳂ꂽ���ǂ���
	bool _isStakeBroken;

	BossIronBall* _ironBall;


	Player* _player;
};