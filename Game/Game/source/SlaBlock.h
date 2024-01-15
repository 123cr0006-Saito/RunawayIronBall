#pragma once
#include "EnemyBase.h"
#include "Player.h"
#include <string>
class SlaBlock : public EnemyBase
{
public:
	SlaBlock(int model, VECTOR pos, Player* player);
	~SlaBlock();

	virtual bool ModeAttack()override;
	virtual bool ModeCoolTime() override;

protected:
	static const float _fixSartchSize;//索敵範囲の半径
	static const float _fixDiscoverSize;//発見時、対象の見失うまでの距離の半径
	static const float _fixAttackSize;//発見時、攻撃モーションに入るまでの範囲
};

