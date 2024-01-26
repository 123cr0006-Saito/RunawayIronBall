#pragma once
#include "EnemyBase.h"
//��]�L�͈͍U��
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

	int _nowAttackDistance;//���ǂꂮ�炢���Ɉړ����Ă��邩
	int _attackDistanceSpeed;//�U�����̔��a�̈ړ��X�s�[�h

	VECTOR _attackPos;//�U�����ɑ�����l
	float _attackDir;//�U���������Ɏg�p����p�x
};