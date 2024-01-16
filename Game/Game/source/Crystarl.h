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
	static const float _fixSartchSize;//���G�͈͂̔��a
	static const float _fixDiscoverSize;//�������A�Ώۂ̌������܂ł̋����̔��a
	static const float _fixAttackSize;//�������A�U�����[�V�����ɓ���܂ł͈̔�

	VECTOR _attackPos;//�U�����ɑ�����l
	float _attackDir;//�U���������Ɏg�p����p�x
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
	static const float _fixSartchSize;//���G�͈͂̔��a
	static const float _fixDiscoverSize;//�������A�Ώۂ̌������܂ł̋����̔��a
	static const float _fixAttackSize;//�������A�U�����[�V�����ɓ���܂ł͈̔�

	int _nowAttackDistance;//���ǂꂮ�炢���Ɉړ����Ă��邩
	int _attackDistanceSpeed;//�U�����̔��a�̈ړ��X�s�[�h

	VECTOR _attackPos;//�U�����ɑ�����l
	float _attackDir;//�U���������Ɏg�p����p�x
};
