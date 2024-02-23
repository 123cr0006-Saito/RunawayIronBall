#pragma once
#include "EnemyBase.h"
//‰ñ“]L”ÍˆÍUŒ‚
class CrystarPattern4 : public EnemyBase
{
public:
	CrystarPattern4();
	~CrystarPattern4();
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