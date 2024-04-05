//----------------------------------------------------------------------
// @filename SlaBlockPattern2.h
// ＠date: 2024/01/26
// ＠author: saito ko
// @explanation
// スラブロックの攻撃パターン2(ガラス,鉄)のクラス 1度だけプレイヤーの真上から落下攻撃を行います
//----------------------------------------------------------------------
#pragma once
#include "EnemyBase.h"
#include "ScreenVibration.h"
#include "Player.h"
#include <string>
class SlaBlockPattern2 : public EnemyBase
{
public:
	SlaBlockPattern2();
	~SlaBlockPattern2()override;

	void InheritanceInit()override;
	void AnimInit()override;
	void CommandProcess()override;

	bool ModeSearch(bool plAttack)override;
	bool ModeDisCover()override;
	bool ModeAttack()override;
	bool ModeCoolTime()override;
	bool ModeKnockBack()override;

	bool IndividualProcessing()override;
	bool IndividualRendering()override;

	bool SetGravity()override;
	bool DebugRender()override;

	VECTOR GetCollisionPos()override { return MV1GetFramePosition(_model, _collisionFrame); }

protected:
	int _fallCount;
	static int _collisionFrame;
	static enum ANIMSTATE : int {
		IDLE = 0,
		WALK,
		DROP,
		DROPLOOP,
		STOMP,
		STAN
	};
	ANIMSTATE _animState;
};

