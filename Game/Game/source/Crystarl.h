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

	VECTOR _attackPos;//UŒ‚‚É‘‚¦‚é’l
	float _attackDir;//UŒ‚‚µ‚½‚Ég—p‚·‚éŠp“x
};

