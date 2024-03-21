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


	VECTOR GetStakePosition() { return _stakePos; }
	VECTOR GetIBPosition() { return _ironBall->GetPosition(); }

	void SetIBPosition(VECTOR pos) { _ironBall->SetPosition(pos); }
	bool GetIsGlass() { return _ironBall->GetIsGlass(); }

	Capsule GetStakeCollision() { return _stakeCapsuleCol; }
	Sphere GetIBCollision() { return _ironBall->GetIBCollision(); }
	Capsule GetChainCollision() { return _ironBall->GetChainCollision(); }
	// 鉄球の回転攻撃かどうかを取得する
	bool GetIsRotationAttack() { return _ironBall->GetIsRotationAttack(); }

	void SetOnStage(bool isOnStage) { _ironBall->SetOnStage(isOnStage); }
	bool GetOnStage() { return _ironBall->GetOnStage(); }

	bool GetIBInvincible() { return _ironBall->GetIsInvincible(); }

	void CheckHitBossAndStake();

	void SetIBKnockBack(VECTOR vDir, float speed) { _ironBall->SetKnockBack(vDir, speed); }
	// 杭のHPを減らす
	void SetDamageStake(int damage);

	// 杭のHPを取得
	int GetStakeHp() { return _stakeHp; }
	// 杭の最大HPを取得
	int GetStakeMaxHp() { return STAKE_MAX_HP; }

	bool GetIsStakeBroken() { return _isStakeBroken; }

	void SetStageRadius(float radius) { _ironBall->SetStageRadius(radius); }

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
	// 杭が無敵状態かどうか
	bool _isStakeInvincible;
	// 杭の無敵時間
	int _stakeInvincibleCnt;
	// 杭が破壊されたかどうか
	bool _isStakeBroken;

	BossIronBall* _ironBall;


	Player* _player;
};