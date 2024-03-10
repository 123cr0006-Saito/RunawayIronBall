#pragma once
#include "EnemyBase.h"
#include "ScreenVibration.h"
#include "Player.h"
#include <string>
class SlaBlock : public EnemyBase
{
public:
	SlaBlock();
	~SlaBlock()override;

	void InheritanceInit()override;
	void AnimInit()override;
	void CommandProcess()override;

	bool ModeSearch(bool plAttack)override;
	bool ModeDisCover()override;
	bool ModeAttack()override;
	bool ModeCoolTime()override;
	bool ModeKnockBack()override;

	bool SetGravity()override;

	bool IndividualProcessing()override;
	bool IndividualRendering()override;

	VECTOR GetCollisionPos()override { return MV1GetFramePosition(_model, _collisionFrame); }

protected : 
	static enum ANIMSTATE : int {
		IDLE = 0,
		WALK,
		DROP,
		DROPLOOP,
		STOMP,
		STAN
	};
	static int _collisionFrame;
	ANIMSTATE _animState;
};