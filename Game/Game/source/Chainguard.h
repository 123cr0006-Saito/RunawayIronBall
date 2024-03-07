#pragma once
#include "EnemyBase.h"
class Chainguard : public EnemyBase
{
public:
	Chainguard();
	~Chainguard()override;
	void InheritanceInit()override;  //継承先専用の初期化
	void AnimInit()override; // アニメーションの初期化
	void CommandProcess()override;// コマンドがあった場合処理

	bool ModeSearch()override; // サーチ状態
	bool ModeAttack()override; // 攻撃状態
	bool ModeCoolTime()override; // クールタイム状態
	bool ModeKnockBack()override; // ノックバック状態

	bool SetGravity()override; // 重力処理　変更すると思うのでありますが使用しなかった場合削除しても大丈夫です。
	void SetKnockBackAndDamage(VECTOR vDir, float damage)override;// ノックバックの処理　ノックバックをしない状態になっているのでダメージの処理です。

	bool IndividualProcessing()override; // 継承先専用のプロセスです。 とりあえずアニメーションの処理をしています。
	bool IndividualRendering()override; // 継承先専用の描画関数です。 鉄球などあると思うので使用してください

protected:
	static enum ANIMSTATE : int {
		ROTATE_START = 0,
		ROTATE_LOOP,
		ROTATE_END,
		WALK,
		IDLE,
		SWING,
		SWING_STIFF,
		SWING_BACK,
		REPALLED,
		STAN,
		WAKE_UP
	};

	ANIMSTATE _animState;
};