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


	VECTOR GetStakePosition() { return _stakePos; }
	VECTOR GetIBPosition() { return _ironBall->GetPosition(); }

	void SetIBPosition(VECTOR pos) { _ironBall->SetPosition(pos); }
	bool GetIsGlass() { return _ironBall->GetIsGlass(); }

	Capsule GetStakeCollision() { return _stakeCapsuleCol; }
	Sphere GetIBCollision() { return _ironBall->GetIBCollision(); }
	Capsule GetChainCollision() { return _ironBall->GetChainCollision(); }
	// “S‹…‚Ì‰ñ“]UŒ‚‚©‚Ç‚¤‚©‚ğæ“¾‚·‚é
	bool GetIsRotationAttack() { return _ironBall->GetIsRotationAttack(); }

	void SetOnStage(bool isOnStage) { _ironBall->SetOnStage(isOnStage); }
	bool GetOnStage() { return _ironBall->GetOnStage(); }

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

	void SetStageRadius(float radius) { _ironBall->SetStageRadius(radius); }

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