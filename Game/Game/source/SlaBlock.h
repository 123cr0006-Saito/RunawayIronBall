#pragma once
#include "EnemyBase.h"
#include "Player.h"
#include <string>
class SlaBlock : public EnemyBase
{
public:
	SlaBlock();
	~SlaBlock();

	void InheritanceInit()override;
	bool ModeAttack()override;
	bool ModeCoolTime()override;

};