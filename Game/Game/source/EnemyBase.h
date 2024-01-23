#pragma once
#include "appframe.h"
#include "Player.h"
#include "math.h"
//�G�l�~�[�e��̂��ƂɂȂ�N���X
class EnemyBase
{
public:
	EnemyBase(int model, VECTOR pos);
	~EnemyBase();

	bool Process();
	bool Render();

	virtual bool DebugRender();

	virtual bool ModeSearch();
	virtual bool ModeDisCover();
	virtual bool ModeAttack();
	virtual bool ModeCoolTime();

	virtual bool SetState();

	bool StopPos();

	virtual VECTOR GetCollisionPos() { return VAdd(_pos, _diffeToCenter); }
	float GetR() { return _r; }

protected:
	Player* _player;
	//��ȕϐ�
	int    _model;//���f��
	VECTOR _pos;//�G�l�~�[�̍��W
	int    _speed;//�ړ����x
	float _direction;//y���̌����Ă������
	float _r;//�����蔻��p�̔��a
	VECTOR _diffeToCenter;//�R���W�����Ɏg�p���钆�S�_�܂ł̍���

	//�b���ۑ��p�ϐ�
	float _stopTime;//��~���Ă��鎞��
	int    _currentTime;//�ړ��n�_�ɂ������̎���

	//���W�i�[�n�ϐ�
	VECTOR _orignPos;//�o���n�_
	VECTOR _nextMovePoint;//���̈ړ��n�_
	VECTOR _savePos;//�����̈ʒu���i�[

	//���G�n�ϐ�
	static const float _flontAngle;//���E�͈͂̊p�x
	float _sartchRange;//���o�͈͂̔��a
	float _moveRange;//�ړ��͈͂̔��a
	float _sartchRangeSize;//���G�͈͂̔��a
	float _discoverRangeSize;//�������A�Ώۂ̌������܂ł̋����̔��a
	float _attackRangeSize;//���ʂ̍U���͈�

	//��������邽�߂̕ϐ�
	float _nextDir;//�������Ƃ��̎��̊p�x
	float _oldDir;//�������O�Ɋi�[����p�x

	//�C�[�W���O�����邽�߂̕ϐ�
	int _easingFrame;//�C�[�W���O�̃t���[���i�[�ϐ�
	VECTOR _saveNextPoint;//���Ɉړ�����n�_

	//�G�̏��
	enum TYPE : int {
		search,
		discover,
		attack,
		cooltime
	};

	TYPE _state;//���̏��

};

//�G�̂悤�ȑ��ʂ�vector�z������Ƃ���erase�͍Ō�̗v�f�ƌ������Ă���erase�g�p
//�������Ԃ��������܂��B
//�G��������z��̒��g���o���o���ɂȂ��Ă����v�ł��傤�E�E�E