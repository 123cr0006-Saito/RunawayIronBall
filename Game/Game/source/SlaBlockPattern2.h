#pragma once
#include "EnemyBase.h"
#include "Player.h"
#include <string>
class SlaBlockPattern2 : public EnemyBase
{
public:
	SlaBlockPattern2();
	~SlaBlockPattern2();

	void InheritanceInit()override;
	bool ModeAttack()override;
	bool ModeCoolTime()override;
	bool SetGravity()override;
	bool DebugRender()override;
protected:
	int _fallCount;
};

