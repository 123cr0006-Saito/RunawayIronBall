#pragma once
#include "appframe.h"
#include "Player.h"
#include "math.h"
#include "EnemyStract.h"
#include "BoardPolygon.h"
#include "EffectManeger.h"
#include "Suppression.h"

#include <string>

#define EN_MOTION_CHANGE 0

//�G�l�~�[�e��̂��ƂɂȂ�N���X
class EnemyBase
{
public:
	EnemyBase();
	~EnemyBase();

	bool Create(int model, VECTOR pos, EnemyParam param,std::string name);
	virtual void Init(VECTOR pos, float scale);
	virtual void Init(VECTOR pos);
	virtual void InheritanceInit();
	virtual void AnimInit();

	void SetPos(VECTOR pos);
	void SetKindPos(VECTOR pos);

	bool Process();
	bool Render();

	virtual void CommandProcess();

	virtual bool DebugRender();

	virtual bool ModeSearch();
	virtual bool ModeSearchToTurn();
	virtual bool ModeSearchToMove();
	virtual bool ModeSearchToCoolTime();


	virtual bool ModeDisCover();
	virtual bool ModeAttack();
	virtual bool ModeCoolTime();
	virtual bool ModeKnockBack();
	virtual bool ModeDead();

	virtual bool IndividualProcessing();
	virtual bool IndividualRendering();
	virtual bool SetState();
	virtual bool SetGravity();

	void SetKnockBack(VECTOR vDir, float damage);//�U�����󂯂����̏���

	bool GetUse() { return _IsUse; }
	virtual VECTOR GetCollisionPos() { return VAdd(_pos, _diffeToCenter); }
	VECTOR GetRotation() { return _rotation; }
	float GetR() { return _r; }
	ENEMYTYPE GetEnemyState() { return _modeState; }

	void SetExtrusionPos(VECTOR movePos) { _pos = VAdd(_pos, movePos); }

protected:
	Player* _player;
	bool _IsUse;

	//Json�œǂݍ��ނ���------------------------------------------------------------------------

	int _hp;//�G�̗̑�
	int _maxHp;//�G�̗̑͂̍ő�l
	int _weightExp;//�G���瓾����o���l
	float  _speed;//�ړ����x
	int _coolTime;//�U����̃N�[���^�C��

	//���G�n�ϐ�
	float _flontAngle;//���E�͈͂̊p�x
	float _sartchRange;//���G�͈͂̔��a
	float _moveRange;//�ړ��͈͂̔��a
	float _hearingRangeSize;//���o�͈͂̔��a
	float _discoverRangeSize;//�������A�Ώۂ̌������܂ł̋����̔��a
	float _attackRangeSize;//���ʂ̍U���͈�
	int _suppression; // �����l

	//------------------------------------------------------------------------------------------------
	
	//��ȕϐ�
	int    _model;//���f��
	VECTOR _pos;//�G�l�~�[�̍��W
	VECTOR _forwardVec; // ���ʕ����̃x�N�g��
	float _gravity;//�d�͉����x
	VECTOR _rotation;//y���̌����Ă������
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

	//�m�b�N�o�b�N������Ƃ��Ɉړ�����
	VECTOR _knockBackDir;//�G�l�~�[���U�����ꂽ�Ƃ��ړ����Ă��������x�N�g��
	int _knockBackSpeedFrame;//�G�l�~�[���U�����ꂽ�Ƃ��Ɉړ�����speed�ƃt���[��

	ENEMYTYPE _modeState;// ���݂̂̏��
	SEARCHTYPE _searchState; //Search��Ԃ̒��̏��

	// �A�j���[�V�����}�l�[�W���[
	AnimationManager* _animManager;
	// �t���[���f�[�^
	FrameData* _frameData;

};

//�G�̂悤�ȑ��ʂ�vector�z������Ƃ���erase�͍Ō�̗v�f�ƌ������Ă���erase�g�p
//�������Ԃ��������܂��B
//�G��������z��̒��g���o���o���ɂȂ��Ă����v�ł��傤�E�E�E