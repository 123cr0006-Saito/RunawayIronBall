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
	bool GetIBInvincible() { return _ironBall->GetIsInvincible(); }

	void CheckHitBossAndStake();

	void SetKnockBack(VECTOR vDir, float speed) { _ironBall->SetKnockBack(vDir, speed); }
	// �Y��HP�����炷
	void SetDamageStake(int damage) {
		_stakeHp -= damage;
		if (_stakeHp < 0) { 
			_stakeHp = 0; 
			_isStakeBroken = true;
			_ironBall->SetISStakeBroken(true);
		}
	}
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
	// �Y���j�󂳂ꂽ���ǂ���
	bool _isStakeBroken;

	BossIronBall* _ironBall;


	Player* _player;
};