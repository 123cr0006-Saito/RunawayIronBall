#pragma once
#include "EnemyBase.h"
#include "Player.h"
#include <string>
class SlaBlock : public EnemyBase
{
public:
	SlaBlock(int model, VECTOR pos, Player* player);
	~SlaBlock();

	bool ModeAttack()override;
	bool ModeCoolTime()override;

protected:
	static const float _fixSartchSize;//���G�͈͂̔��a
	static const float _fixDiscoverSize;//�������A�Ώۂ̌������܂ł̋����̔��a
	static const float _fixAttackSize;//�������A�U�����[�V�����ɓ���܂ł͈̔�
};