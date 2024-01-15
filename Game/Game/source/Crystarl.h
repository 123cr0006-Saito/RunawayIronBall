#pragma once
#include "EnemyBase.h"
class Crystarl : public EnemyBase
{
public:
	Crystarl(int model, VECTOR pos, Player* player);
	~Crystarl();
	bool ModeSearch()override;
	bool ModeAttack()override;
	bool ModeCoolTime()override;
	bool SetState()override;
protected:
	static const float _fixSartchSize;//索敵範囲の半径
	static const float _fixDiscoverSize;//発見時、対象の見失うまでの距離の半径
	static const float _fixAttackSize;//発見時、攻撃モーションに入るまでの範囲

	VECTOR _attackPos;//攻撃時に増える値
	float _attackDir;//攻撃した時に使用する角度
};

class CrystarlPattern2 : public EnemyBase
{
public:
	CrystarlPattern2(int model, VECTOR pos, Player* player);
	~CrystarlPattern2();
	bool ModeAttack()override;
	bool ModeCoolTime()override;
	bool SetState()override;
protected:
	static const float _fixSartchSize;//索敵範囲の半径
	static const float _fixDiscoverSize;//発見時、対象の見失うまでの距離の半径
	static const float _fixAttackSize;//発見時、攻撃モーションに入るまでの範囲

	int _nowAttackDistance;//今どれぐらい横に移動しているか
	int _attackDistanceSpeed;//攻撃時の半径の移動スピード

	VECTOR _attackPos;//攻撃時に増える値
	float _attackDir;//攻撃した時に使用する角度
};
