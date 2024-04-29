//----------------------------------------------------------------------
// @filename Boss.h
// @date: 2024/03/20
// @author: Morozumi Hiroya
// @explanation
// ボスの制御を行うクラス
// ボス鉄球の制御は、BossIronBallクラスで行う
//----------------------------------------------------------------------
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

	// モデルの読み込み
	void LoadModel();
	// 初期化処理
	void Init(VECTOR polePos);
	// 更新処理
	void Process();
	// 描画処理
	void Render();

	// 杭の座標を取得
	VECTOR GetStakePosition() { return _stakePos; }
	// ボス鉄球の座標を取得
	VECTOR GetIBPosition() { return _ironBall->GetPosition(); }
	// ボス鉄球の座標を設定
	void SetIBPosition(VECTOR pos) { _ironBall->SetPosition(pos); }

	// ガラス化状態かどうかを取得する
	bool GetIsGlass() { return _ironBall->GetIsGlass(); }

	// 杭が無敵状態かどうかを取得する
	bool GetIsStakeInvincible() { return _isStakeInvincible; }

	// 杭の当たり判定を取得する
	Capsule GetStakeCollision() { return _stakeCapsuleCol; }
	// ボス鉄球の当たり判定を取得する
	Sphere GetIBCollision() { return _ironBall->GetIBCollision(); }
	// ボス鉄球の鎖の当たり判定を取得する
	Capsule GetChainCollision() { return _ironBall->GetChainCollision(); }

	// ボス鉄球が回転攻撃かどうかを取得する
	bool GetIsRotationAttack() { return _ironBall->GetIsRotationAttack(); }

	// ボス鉄球がステージ上にあるかどうかを設定する
	void SetOnStage(bool isOnStage) { _ironBall->SetOnStage(isOnStage); }
	// ボス鉄球がステージ上にあるかどうかを取得する
	bool GetOnStage() { return _ironBall->GetOnStage(); }

	// ボス鉄球の無敵状態かどうかを取得する
	bool GetIBInvincible() { return _ironBall->GetIsInvincible(); }

	// ボス鉄球と杭の当たり判定を行う 
	void CheckHitBossAndStake();
	// ボス鉄球をノックバック状態にする
	void SetIBKnockBack(VECTOR vDir, float speed) { _ironBall->SetKnockBack(vDir, speed); }
	// 杭のHPを減らす
	void SetDamageStake(int damage);

	// 杭のHPを取得
	int GetStakeHp() { return _stakeHp; }
	// 杭の最大HPを取得
	int GetStakeMaxHp() { return STAKE_MAX_HP; }

	// 杭が破壊されたかどうかを取得
	bool GetIsStakeBroken() { return _isStakeBroken; }

	// ステージの半径を設定
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

	// 鉄球
	BossIronBall* _ironBall;

	// プレイヤー
	Player* _player;
};