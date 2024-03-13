#pragma once
#include "appframe.h"
#include "CharacterBase.h"
#include "bone.h"
#include "myJson.h"

#include "IronBall.h"

#include "AnimationManager.h"
#include "AnimationItem.h"
#include "EffectManeger.h"
#include "EffekseerPosSynchro.h"

#include "FrameData.h"

#include "ModelColor.h"



class Player : public CharacterBase
{
private:
	enum class ANIM_STATE {
		IDLE,
		IDLE_TIRED,
		WALK,
		WALK_TIRED,
		RUN,

		HORISONTAL_SWING_01,
		HORISONTAL_SWING_02,
		HORISONTAL_SWING_03,

		TO_ROTATION_SWING,
		ROTATION_SWING,

		IDLE_FIGHTING,

		GAMEOVER,

		AVOIDANCE,
		HIT,
		WIN,
	};

public:
	Player();
	~Player() override;

	bool Init(int modelHandle, VECTOR pos) override;
	bool Process(float camAngleY);
	bool AnimationProcess();
	bool BlastOffProcess();
	bool Render() override;


	int GetHP() { return _hp; }
	bool GetIsInvincible() { return _isInvincible; }
	// ���G��Ԃ̍X�V
	void ChangeIsInvincible(bool b, int frame);

	void SetDamage();


	// �L�������f���̓_�ŏ���
	void FlickerProcess();



	float GetStamina() { return _stamina; }
	float GetStaminaMax() { return _staminaMax; }
	float GetStaminaRate() { return _stamina / _staminaMax; }





	void SetBone();//�V����������֐��ł��B bone�̃t���[����T�����߂Ɏg�p����֐��ł��B���json�ł̓ǂݍ��݂ɂ��邩������܂���B
	//���V����������֐��ł��B�ǂ���json�ǂݍ��݂��ǂ��ɏ����΂悢�̂��킩��Ȃ������̂ŁA�R���X�g���N�^�̎��ɌĂяo���֐��Ƃ��Ď������܂����B
	void SetNextExp(std::string FileName);//�o���l�f�[�^�̓ǂݍ���
	bool HealHp();
	bool  UpdateExp();//�o���l���z���Ă������A���x�����グ��B
	int GetNowLevel() { return _nowLevel; };
	void SetExp(int getExp) { _nowExp += getExp; };
	//�o���lUI�Ŏg�p���Ă��܂��B
	int GetNowExp() { return _nowExp; }
	int GetNextExp() { return _nextLevel[_nowLevel]; }


	void SetPowerScale(std::string FileName);//�t�@�C���ǂݍ��݂Ń��x���ɍ��킹���U���͂Ɗg�嗦���擾
	bool UpdateLevel();// ���x���A�b�v���ɍU���͂Ɗg�嗦��ݒ�
	int GetPower() { return _power; }//�m�b�N�o�b�N�p�̗͂�Ԃ��܂��B



	void UpdateBone();
	void UpdateCollision();

	Capsule GetCollision() { return _capsuleCollision; };
	Sphere GetIBCollision() { return _ironBall->GetIBCollision(); };
	VECTOR GetIBPos() { return _ironBall->GetBallPosition(); };
	void SetIBPos(VECTOR pos) { _ironBall->SetBallPosition(pos); };

	void SetBlastOffPower(VECTOR dir, float power) { _blastOffDir = dir; _blastOffPower = power; };



	VECTOR GetRightHandPos();

	VECTOR* GetIBPosPtr() { return _ironBall->GetBallPosPtr(); }


	bool GetAttackState() { return _isAttackState; }
	bool GetEnabledIBAttackCollision() { return _ironBall->GetEnabledAttackCollision(); }

	// �t���[���f�[�^�̃R�}���h���`�F�b�N����
	void CheckFrameDataCommand();

	static Player* GetInstance() { return _instance; }

	void DrawDebugInfo();

	VECTOR GetStickDir() { return _stickDir; }
private:
	// ���͏��
	XInput* _input;
	// L�X�e�B�b�N�̓��͕���
	// L�X�e�B�b�N���͂��������ꍇ�ɍX�V����
	VECTOR _stickDir;

	/* �X�e�[�^�X�֘A */
	// HP
	int _hp;
	// ���G���ǂ���
	bool _isInvincible;
	// �c��̖��G����
	int _invincibleRemainingCnt;
	// �퓬�ҋ@��Ԃ̎c��t���[����
	int _idleFightingRemainingCnt;

	// �X�^�~�i
	float _stamina;
	// �X�^�~�i�̍ő�l
	float _staminaMax;
	// �X�^�~�i��������ǂ���
	bool _isConsumingStamina;
	// �X�^�~�i���s�������ǂ���
	bool _isTired;
	// �X�^�~�i��1�t���[��������̉񕜑��x
	float _staminaRecoverySpeed;

	// �ړ��\���ǂ���
	bool _canMove;
	// �ړ����x
	float _moveSpeed;
	// �ړ����x�i�t���[���f�[�^���g�����ړ��j
	float _moveSpeedFWD;

	// �U����Ԃ��ǂ���
	bool _isAttackState;
	// �ʏ�U�������ǂ���
	bool _isSwinging;
	// ��]�U�������ǂ���
	bool _isRotationSwinging;
	// ��]�U���̃{�^��������������Ă���t���[����
	int _rotationCnt;

	/* �A�j���[�V�����֘A */
	// ���[�V�����ύX�\���ǂ���
	bool  _canMotionCancel;
	// ���̃R���{���[�V�������Đ����邩�ǂ���
	bool _playNextComboAnim;

	// �A�j���[�V�����}�l�[�W��
	AnimationManager* _animManager;
	// �A�j���[�V�������̃}�b�v�R���e�i
	static std::map<int, ANIMATION_INFO> _animMap;

	// �A�j���[�V�����̃X�e�[�g
	ANIM_STATE _animStatus;

	// �t���[���f�[�^
	FrameData* _frameData;

	// �S��
	IronBall* _ironBall;

	// �����蔻��
	Capsule _capsuleCollision;

	VECTOR _blastOffDir;
	float _blastOffPower;


	int _rightHandFrameIndex;




	// ��_���[�W���̃��f���_�ŏ������s���N���X
	ModelColor* _modelColor;



	//------------
	//�V���������܂����B
	bone* _bone[2];// 0:leftHear 1:RightHear
	int _nowLevel;//���݂̃��x��������܂��B
	int _nowExp; //���ݎ����Ă���o���l���i�[���܂��B
	int _maxLevel;//���x���̍ő�l
	std::map<int, int> _nextLevel;// first ���݂̃��x��  second  ���̃��x�����オ��܂ł̌o���l

	int _power;//������΂��͂ł��B
	std::map<int, std::pair<int, float>> _powerAndScale;//�U���͂Ɗg�嗦���i�[�����R���e�i�ł��B
	//------------


	static Player* _instance;
};