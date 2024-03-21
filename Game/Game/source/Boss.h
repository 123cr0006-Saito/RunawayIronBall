#pragma once
#include "appframe.h"
#include "BossIronBall.h"
#include "Player.h"

namespace {
	// Y‚ÌÅ‘åHP
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
	// Y‚ÌHP‚ğŒ¸‚ç‚·
	void SetDamageStake(int damage);

	// Y‚ÌHP‚ğæ“¾
	int GetStakeHp() { return _stakeHp; }
	// Y‚ÌÅ‘åHP‚ğæ“¾
	int GetStakeMaxHp() { return STAKE_MAX_HP; }

	bool GetIsStakeBroken() { return _isStakeBroken; }

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
	// Y‚ª–³“Gó‘Ô‚©‚Ç‚¤‚©
	bool _isStakeInvincible;
	// Y‚Ì–³“GŠÔ
	int _stakeInvincibleCnt;
	// Y‚ª”j‰ó‚³‚ê‚½‚©‚Ç‚¤‚©
	bool _isStakeBroken;

	BossIronBall* _ironBall;


	Player* _player;
};