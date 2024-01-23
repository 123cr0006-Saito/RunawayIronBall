#pragma once
#include "appframe.h"
#include "Player.h"
#include "math.h"
#include "EnemyStract.h"

//�G�l�~�[�e��̂��ƂɂȂ�N���X
class EnemyBase
{
public:
	EnemyBase();
	~EnemyBase();

	bool Create(int model, VECTOR pos, EnemyParam param);
	virtual void Init(VECTOR pos, float scale);
	virtual void Init(VECTOR pos);
	virtual void InheritanceInit();
	//---------------------------------------------------------
	//�f�o�b�O�p�̊֐��ł��B�f�ނ��������ŏ����܂�
	void  DebugSnail();
	//---------------------------------------------------------
	void SetPos(VECTOR pos);

	bool Process();
	bool Render();

	virtual bool DebugRender();

	virtual bool ModeSearch();
	virtual bool ModeDisCover();
	virtual bool ModeAttack();
	virtual bool ModeCoolTime();

	virtual bool SetState();

	bool StopPos();

	bool GetUse() { return _IsUse; }
	virtual VECTOR GetCollisionPos() { return VAdd(_pos, _diffeToCenter); }
	float GetR() { return _r; }

protected:
	Player* _player;
	bool _IsUse;

	//Json�œǂݍ��ނ���------------------------------------------------------------------------

	int _hp;//�G�̗̑�
	int _exp;//�G���瓾����o���l
	float  _speed;//�ړ����x
	int _coolTime;//�U����̃N�[���^�C��

	//���G�n�ϐ�
	float _flontAngle;//���E�͈͂̊p�x
	float _sartchRange;//���G�͈͂̔��a
	float _moveRange;//�ړ��͈͂̔��a
	float _hearingRangeSize;//���o�͈͂̔��a
	float _discoverRangeSize;//�������A�Ώۂ̌������܂ł̋����̔��a
	float _attackRangeSize;//���ʂ̍U���͈�

	//------------------------------------------------------------------------------------------------
	
	//��ȕϐ�
	int    _model;//���f��
	VECTOR _pos;//�G�l�~�[�̍��W
	float _direction;//y���̌����Ă������
	float _r;//�����蔻��p�̔��a
	float _scale;//�G�̃T�C�Y
	VECTOR _diffeToCenter;//�R���W�����Ɏg�p���钆�S�_�܂ł̍���

	//�b���ۑ��p�ϐ�
	float _stopTime;//��~���Ă��鎞��
	int    _currentTime;//�ړ��n�_�ɂ������̎���

	//���W�i�[�n�ϐ�
	VECTOR _orignPos;//�o���n�_
	VECTOR _nextMovePoint;//���̈ړ��n�_
	VECTOR _savePos;//�����̈ʒu���i�[

	//��������邽�߂̕ϐ�
	float _nextDir;//�������Ƃ��̎��̊p�x
	float _oldDir;//�������O�Ɋi�[����p�x

	//�C�[�W���O�����邽�߂̕ϐ�
	int _easingFrame;//�C�[�W���O�̃t���[���i�[�ϐ�
	VECTOR _saveNextPoint;//���Ɉړ�����n�_

	//�G�̏��
	enum class ENEMYTYPE : int {
		SEARCH,
		DISCOVER,
		ATTACK,
		COOLTIME
	};



	ENEMYTYPE _state;//���̏��

};

//�G�̂悤�ȑ��ʂ�vector�z������Ƃ���erase�͍Ō�̗v�f�ƌ������Ă���erase�g�p
//�������Ԃ��������܂��B
//�G��������z��̒��g���o���o���ɂȂ��Ă����v�ł��傤�E�E�E