#pragma once
#include "EnemyBase.h"
class Crystarl : public EnemyBase
{
public:
	Crystarl();
	~Crystarl();
	void  DebugSnail()override;

	void InheritanceInit()override;
	bool ModeSearch()override;
	bool ModeAttack()override;
	bool ModeCoolTime()override;
	bool ModeKnockBack()override;
	bool SetState()override;

	bool DebugRender()override;
	VECTOR GetCollisionPos()override { return VAdd(VAdd(_pos, _diffeToCenter), _attackPos); }
protected:

	VECTOR _attackPos;//�U�����ɑ�����l
	float _attackDir;//�U���������Ɏg�p����p�x
};

