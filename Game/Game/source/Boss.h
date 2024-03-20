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
	// Y‚ÌHP‚ğŒ¸‚ç‚·
	void SetDamageStake(int damage) {
		_stakeHp -= damage;
		if (_stakeHp < 0) { 
			_stakeHp = 0; 
			_isStakeBroken = true;
			_ironBall->SetISStakeBroken(true);
		}
	}
	// Y‚ÌHP‚ğæ“¾
	int GetStakeHp() { return _stakeHp; }

	// ƒfƒoƒbƒOî•ñ‚Ì•\¦
	void DrawDebugInfo();

private:
	// Y‚Ìƒ‚ƒfƒ‹ƒnƒ“ƒhƒ‹
	int _stakeModelHandle;
	// Y‚ÌÀ•W
	VECTOR _stakePos;
	// Y‚Ì“–‚½‚è”»’è
	Capsule _stakeCapsuleCol;
	// Y‚ÌHP
	int _stakeHp;
	// Y‚ª”j‰ó‚³‚ê‚½‚©‚Ç‚¤‚©
	bool _isStakeBroken;

	BossIronBall* _ironBall;


	Player* _player;
};