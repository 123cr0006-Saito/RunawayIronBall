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
protected:
	int _fallCount;
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

