#pragma once
#include "appframe.h"
#include "BossIronBall.h"
#include "Player.h"

namespace {
	// 杭の最大HP
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

	void SetKnockBack(VECTOR vDir, float speed) { _ironBall->SetKnockBack(vDir, speed); }
	// 杭のHPを減らす
	void SetDamageStake(int damage) {
		_stakeHp -= damage;
		if (_stakeHp < 0) { 
			_stakeHp = 0; 
			_isStakeBroken = true;
			_ironBall->SetISStakeBroken(true);
		}
	}

	// 杭のHPを取得
	int GetStakeHp() { return _stakeHp; }
	// 杭の最大HPを取得
	int GetStakeMaxHp() { return STAKE_MAX_HP; }

	// デバッグ情報の表示
	void DrawDebugInfo();

private:
	// 杭のモデルハンドル
	int _stakeModelHandle;
	// 杭の座標
	VECTOR _stakePos;
	// 杭の当たり判定
	Capsule _stakeCapsuleCol;
	// 杭のHP
	int _stakeHp;
	// 杭が破壊されたかどうか
	bool _isStakeBroken;

	BossIronBall* _ironBall;


	Player* _player;
};