#pragma once
#include "EnemyBase.h"
class Crystarl : public EnemyBase
{
public:
	Crystarl();
	~Crystarl();
	void InheritanceInit()override;
	bool ModeSearch()override;
	bool ModeAttack()override;
	bool ModeCoolTime()override;
	bool SetState()override;

	bool DebugRender()override;
	VECTOR GetCollisionPos()override { return VAdd(VAdd(_pos, _diffeToCenter), _attackPos); }
protected:

	VECTOR _attackPos;//UŒ‚‚É‘‚¦‚é’l
	float _attackDir;//UŒ‚‚µ‚½‚Ég—p‚·‚éŠp“x
};

class CrystarlPattern2 : public EnemyBase
{
public:
	CrystarlPattern2();
	~CrystarlPattern2();
	void InheritanceInit()override;
	bool ModeAttack()override;
	bool ModeCoolTime()override;
	bool SetState()override;

	bool DebugRender()override;
	VECTOR GetCollisionPos()override { return VAdd(VAdd(_pos, _diffeToCenter), _attackPos); }
protected:

	int _nowAttackDistance;//¡‚Ç‚ê‚®‚ç‚¢‰¡‚ÉˆÚ“®‚µ‚Ä‚¢‚é‚©
	int _attackDistanceSpeed;//UŒ‚‚Ì”¼Œa‚ÌˆÚ“®ƒXƒs[ƒh

	VECTOR _attackPos;//UŒ‚‚É‘‚¦‚é’l
	float _attackDir;//UŒ‚‚µ‚½‚Ég—p‚·‚éŠp“x
};
