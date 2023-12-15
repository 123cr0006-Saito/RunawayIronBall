#pragma once
#include "appframe.h"
#include "Player.h"
#include "math.h"
class EnemyBase
{
public:
	EnemyBase(int model, VECTOR pos);
	~EnemyBase();

	virtual bool Process();
	virtual bool Render();

	virtual bool StopPos();

protected:
	Player* _player;
	VECTOR _pos;//�G�l�~�[�̍��W
	float _direction;//y���̌����Ă������
	float _sartchRange;//���o�͈͂̔��a
	float _moveRange;//�ړ��͈͂̔��a
	int    _model;//���f��
	int    _speed;//�ړ����x
	float _r;//�����蔻��p�̔��a

	float _stopTime;//��~���Ă��鎞��
	int    _arriveTime;//�ړ��n�_�ɂ������̎���

	VECTOR _orignPos;//�o���n�_
	VECTOR _nextMovePoint;//���̈ړ��n�_

	static const float _flontAngle;//���E�͈͂̊p�x
	float _sartchRangeSize;//���G�͈͂̔��a
	float _discoverRangeSize;//�������A�Ώۂ̌������܂ł̋����̔��a;

	//��������邽�߂̕ϐ�
	float _nextDir;
	float _oldDir;

	//�C�[�W���O�����邽�߂̕ϐ�
	int _easingFrame;
	VECTOR _saveNextPoint;

	enum TYPE : int {
		search,
		discover,
		attack
	};

	TYPE _state;//���̏��

};

