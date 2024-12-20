#pragma once
#include "EnemyBase.h"
//回転広範囲攻撃
class CrystarPattern4 : public EnemyBase
{
public:
	CrystarPattern4();
	~CrystarPattern4()override;
	void InheritanceInit()override;
	bool ModeAttack()override;
	bool ModeCoolTime()override;
	bool SetState()override;

	bool DebugRender()override;
	VECTOR GetCollisionPos()override { return VAdd(VAdd(_pos, _diffeToCenter), _attackPos); }
protected:

	int _nowAttackDistance;//今どれぐらい横に移動しているか
	int _attackDistanceSpeed;//攻撃時の半径の移動スピード

	VECTOR _attackPos;//攻撃時に増える値
	float _attackDir;//攻撃した時に使用する角度
};