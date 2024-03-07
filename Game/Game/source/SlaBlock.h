#pragma once
#include "EnemyBase.h"
#include "ScreenVibration.h"
#include "Player.h"
#include <string>
class SlaBlock : public EnemyBase
{
public:
	SlaBlock();
	~SlaBlock();

	void InheritanceInit()override;
	void AnimInit()override;
	void CommandProcess()override;

	bool ModeSearch()override;
	bool ModeDisCover()override;
	bool ModeAttack()override;
	bool ModeCoolTime()override;
	bool ModeKnockBack()override;

	bool SetGravity()override;

	bool IndividualProcessing()override;
	bool IndividualRendering()override;

protected : 
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