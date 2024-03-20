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

	void SetKnockBack(VECTOR vDir = VGet(0.0f, 0.0f, -1.0f)) { _ironBall->SetKnockBack(vDir); }
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