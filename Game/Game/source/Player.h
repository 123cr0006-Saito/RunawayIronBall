#pragma once
#include "appframe.h"
#include "CharacterBase.h"
#include "bone.h"
#include "myJson.h"

#include "AnimationManager.h"
#include "AnimationItem.h"

#include "FrameData.h"


// �e�X�g�p
// �t���[���f�[�^�̃R�}���h
#define C_P_CHANGE_MOTION							0
#define	C_P_ENABLE_MOVE								1
#define C_P_MOVE_SPEED									2
#define C_P_ACCEPT_COMBO_INPUT					3
#define C_P_CHECK_CHANGE_COMBO				4
#define C_P_CHECK_CHANGE_ATTACK_STATE		5

#define C_P_ENABLE_IB_ATTACK_COLLISION		100
#define C_P_ENABLE_IB_FOLLOWING_MODE		101
#define C_P_ENABLE_IB_INTERPOLATION			102

enum IB_MOVE_STATE {
	FOLLOWING,
	PUTTING_ON_SOCKET,
	INTERPOLATION,
};

class Player : public CharacterBase
{
private:
	enum class ANIM_STATE {
		IDLE,		
		RUN,

		HORISONTAL_SWING_01,
		HORISONTAL_SWING_02,
		HORISONTAL_SWING_03,

		MANYTIME_SWING,
		ROTATION_SWING,

		IDLE_FIGHTING,

		LONG_JUMP_AIR,
		LONG_JUMP_NOSEDIVE,
		LONG_JUMP_LANDING,

		GAMEOVER,

		AVOIDANCE,
		HIT,
		IDLE_TIRED,
		WALK_TIRED,
	};

public:
	Player(int modelHandle, VECTOR pos);
	~Player() override;

	bool Process(float camAngleY);
	bool BlastOffProcess();
	bool Render() override;

	void SetBone();//�V����������֐��ł��B bone�̃t���[����T�����߂Ɏg�p����֐��ł��B���json�ł̓ǂݍ��݂ɂ��邩������܂���B
	//���V����������֐��ł��B�ǂ���json�ǂݍ��݂��ǂ��ɏ����΂悢�̂��킩��Ȃ������̂ŁA�R���X�g���N�^�̎��ɌĂяo���֐��Ƃ��Ď������܂����B
	void SetNextExp(std::string FileName);//�o���l�f�[�^�̓ǂݍ���
	bool  UpdateExp();//�o���l���z���Ă������A���x�����グ��B
	int GetNowLevel() { return _nowLevel; };
	void SetExp(int getExp) { _nowExp += getExp; };
	//�o���lUI�Ŏg�p���Ă��܂��B
	int GetNowExp() { return _nowExp; }
	int GetNextExp() { return _nextLevel[_nowLevel]; }

	void UpdateBone();
	void UpdateCollision();

	Capsule GetCollision() { return _capsuleCollision; };


	void SetBlastOffPower(VECTOR dir, float power) { _blastOffDir = dir; _blastOffPower = power; };



	VECTOR GetRightHandPos();

	IB_MOVE_STATE GetIBMoveState() { return _ibMoveState; }


	bool GetIsAttackState() { return _isAttackState; }


	// �t���[���f�[�^�̃R�}���h���`�F�b�N����
	void CheckFrameDataCommand();

	static Player* GetInstance() { return _instance; }

	void DrawDebugInfo();
private:
	// ���͏��
	XInput* _input;
	// L�X�e�B�b�N�̓��͕���
	// L�X�e�B�b�N���͂��������ꍇ�ɍX�V����
	VECTOR _stickDir;

	// �ړ��\���ǂ���
	bool _canMove;
	// �ړ����x
	float _speed;

	Capsule _capsuleCollision;

	// �S�����Ǐ]��Ԃ��ǂ���
	IB_MOVE_STATE _ibMoveState;

	// �U����Ԃ��ǂ���
	bool _isAttackState;

	// ���̃R���{���[�V�������Đ����邩�ǂ���
	bool _playNextComboAnim;

	// �A�j���[�V�����}�l�[�W��
	AnimationManager* _animManager;
	// �A�j���[�V�������̃}�b�v�R���e�i
	static std::map<int, ANIMATION_INFO> _animMap;
	// �퓬�ҋ@��Ԃ̎c��t���[����
	int _idleFightingRemainingCnt;

	ANIM_STATE _animStatus;

	// �t���[���f�[�^
	FrameData* _frameData;


	VECTOR _blastOffDir;
	float _blastOffPower;


	int _rightHandFrameIndex;


	bool _isSwinging;
	bool _isSpinning;
	int _spinCnt;

	static Player* _instance;

	//------------
	//�V���������܂����B
	bone* _bone[2];// 0:leftHear 1:RightHear
	int _nowLevel;//���݂̃��x��������܂��B
	int _nowExp; //���ݎ����Ă���o���l���i�[���܂��B
	int _maxLevel;//���x���̍ő�l
	std::map<int, int> _nextLevel;// first ���݂̃��x��  second  ���̃��x�����オ��܂ł̌o���l
	//------------
};