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

	bool DebugRender()override;
	VECTOR GetCollisionPos()override { return VAdd(VAdd(_pos, _diffeToCenter), _attackPos); }
protected:
	static const float _fixSartchSize;//õ“G”ÍˆÍ‚Ì”¼Œa
	static const float _fixDiscoverSize;//”­Œ©A‘ÎÛ‚ÌŒ©¸‚¤‚Ü‚Å‚Ì‹——£‚Ì”¼Œa
	static const float _fixAttackSize;//”­Œ©AUŒ‚ƒ‚[ƒVƒ‡ƒ“‚É“ü‚é‚Ü‚Å‚Ì”ÍˆÍ

	VECTOR _attackPos;//UŒ‚‚É‘‚¦‚é’l
	float _attackDir;//UŒ‚‚µ‚½‚Ég—p‚·‚éŠp“x
};

class CrystarlPattern2 : public EnemyBase
{
public:
	CrystarlPattern2(int model, VECTOR pos, Player* player);
	~CrystarlPattern2();
	bool ModeAttack()override;
	bool ModeCoolTime()override;
	bool SetState()override;

	bool DebugRender()override;
	VECTOR GetCollisionPos()override { return VAdd(VAdd(_pos, _diffeToCenter), _attackPos); }
protected:
	static const float _fixSartchSize;//õ“G”ÍˆÍ‚Ì”¼Œa
	static const float _fixDiscoverSize;//”­Œ©A‘ÎÛ‚ÌŒ©¸‚¤‚Ü‚Å‚Ì‹——£‚Ì”¼Œa
	static const float _fixAttackSize;//”­Œ©AUŒ‚ƒ‚[ƒVƒ‡ƒ“‚É“ü‚é‚Ü‚Å‚Ì”ÍˆÍ

	int _nowAttackDistance;//¡‚Ç‚ê‚®‚ç‚¢‰¡‚ÉˆÚ“®‚µ‚Ä‚¢‚é‚©
	int _attackDistanceSpeed;//UŒ‚‚Ì”¼Œa‚ÌˆÚ“®ƒXƒs[ƒh

	VECTOR _attackPos;//UŒ‚‚É‘‚¦‚é’l
	float _attackDir;//UŒ‚‚µ‚½‚Ég—p‚·‚éŠp“x
};
