#pragma once
#include "appframe.h"
#include "Player.h"
#include "math.h"
class EnemyBase
{
public: 
	EnemyBase();
	~EnemyBase();

	virtual bool Process();
	virtual bool Render();

	virtual bool StopPos();

	virtual void CollisionDetection();

protected :
	VECTOR _pos;//�G�l�~�[�̍��W
	float _direction;//y���̌����Ă������
	float _sartchRange;//���o�͈͂̔��a
	float _flontAngle;//���E�͈͂̊p�x
	float _moveRange;//�ړ��͈͂̔��a
	int _model;//���f��
	int _speed;//�ړ����x
	float _r;//���a

	float _stopTime;//��~���Ă��鎞��
	int _arriveTime;//�ړ��n�_�ɂ������̎���

	VECTOR _orignPos;//�o���n�_
	VECTOR _nextMovePoint;//���̈ړ��n�_


	enum TYPE : int {
		search,
		discover
	};

	TYPE _state;//���̏��

};

