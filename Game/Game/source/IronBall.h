//----------------------------------------------------------------------
// @filename IronBall.h
// @date: 2024/04/01
// @author: Morozumi Hiroya
// @explanation
// �v���C���[�����S���E���̐���E�`����s���N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "ObjectBase.h"
#include "Afterglow.h"
#include "AfterImage.h"

#define CHAIN_MAX 12

// �ړ����
enum IB_MOVE_STATE {
	// �Ǐ]���
	FOLLOWING,
	// �\�P�b�g�ւ̔z�u���
	PUTTING_ON_SOCKET,
};
class IronBall : public ObjectBase
{
public:
	IronBall();
	virtual ~IronBall();

	void Init();
	// �e���f���̍��W�����Ƃɏ����ʒu��ݒ�
	void InitPosFromParent();
	void Process();

	//----------------------------------------------------------------------
	// �ړ��֘A�̏���

	// �ړ�����
	void MoveProcess();
	// IB_MOVE_STATE�ɉ������������s��
	// �Ǐ]��Ԃ̏���
	void FollowingProcess();
	// �\�P�b�g�ւ̔z�u��Ԃ̏���
	void PuttingOnSocketProcess();
	//----------------------------------------------------------------------

	// �A�j���[�V�����̍X�V����
	// �S���̃A�j���[�V������1��ނ݂̂Ȃ̂�AnimationManager�N���X���g�p���Ȃ�
	void AnimProcess();
	// �`�揈��
	void Render();
	// �c���̕`��
	void RenderAfterImage();

	VECTOR GetBallPosition() { return _iPos; }
	void SetBallPosition(VECTOR pos) { _iPos = pos; }

	VECTOR* GetBallPosPtr() { return &_iPos; }

	bool GetEnabledAttackCollision() { return _enabledAttackCollision; }
	void SetEnabledAttackCollision(bool state) { _enabledAttackCollision = state; }

	Sphere GetIBCollision() { return _ibAttackSphereCollision; }
	Sphere GetIBBodyCollision() { return _ibBodySphereCollision; }
	Capsule GetChainCollision() { return _chainCapsuleCollision; }

	// �S���̓����蔻����X�V
	void UpdateIBCollision();
	// ���̓����蔻����X�V
	void UpdateChainCollision();

	// �ړ���Ԃ�ݒ肷��
	void SetMoveState(IB_MOVE_STATE state) { _moveState = state; }

	// �c����\�����邩�ǂ�����ݒ�
	void SetAddAfterImage(bool enable) { _addAfterImage = enable; }

	// �T�C�g�E���쐬���܂����B
	void SetEnabledAfterGlow(bool enable);// �ڂ̎c����\�����邩�ǂ�����ݒ�

	//----------------------------------------------------------------------
	// ���̃I�u�W�F�N�g��ۗL���Ă���e�I�u�W�F�N�g�֘A�̊֐�

	// �e�I�u�W�F�N�g�̃C���X�^���X�̃Z�b�^�[�E�Q�b�^�[
	void SetParentInstance(ObjectBase* parent) { _parent = parent; }
	ObjectBase* GetParentInstance() { return _parent; }
	// �e�I�u�W�F�N�g�̃��f���n���h�����Z�b�g
	void SetParentModelHandle(int handle);
	//----------------------------------------------------------------------

	// �f�o�b�O���̕\��
	void DrawDebugInfo();


	bool UpdateLevel(float scale);//�v���C���[����擾�����A���x���ōU���͂Ɗg�嗦��ݒ�


private:
	XInput* _input;

	// �S���̃��f���n���h��
	int _iModelHandle;
	// �S���̍��W
	VECTOR _iPos;
	// �S�����f���̐��ʕ���
	VECTOR _iForwardDir;
	// �S�����f���̏����g�嗦
	VECTOR _ibDefaultScale;
	// �ړ����
	IB_MOVE_STATE _moveState;

	// ���̃��f���n���h��
	int _cModelHandle;
	// ���̍��W
	// �Ō�̗v�f�͓S���̍��W�Ƃ���
	VECTOR _cPos[CHAIN_MAX];
	// ���̗֓��m�̊Ԋu
	float _lengthBetweenChains;

	// �S���E���̃��f�����f����z�u����e���f���̃\�P�b�g�ԍ�
	int _socketNo[3];

	//----------------------------------------------------------------------
	// �����蔻��֘A

	// �U�����肪�L�����ǂ���
	bool _enabledAttackCollision;
	// �S�������̓����蔻��`��i�n�ʂƂ̓����蔻��Ɏg���j
	Sphere _ibBodySphereCollision;
	// �S�������̍U�������蔻��
	Sphere _ibAttackSphereCollision;
	// �������̓����蔻��
	Capsule _chainCapsuleCollision;
	// �������̓����蔻���CollisionManager�ɓo�^���邽�߂�Cell
	Cell* _chainCell;
	//----------------------------------------------------------------------

	//----------------------------------------------------------------------
	// �A�j���[�V�����֘A

	// �A�^�b�`�C���f�b�N�X�ԍ�
	int _animIndex;
	// ���Đ�����
	float _animTotalTime;
	// ���݂̍Đ�����
	float _playTime;
	//----------------------------------------------------------------------

	// �c���N���X
	AfterImage* _afterImage;
	// �V�����c���𐶐����邩�ǂ���
	bool _addAfterImage;

	// ���̃I�u�W�F�N�g��ۗL���Ă���e�̃I�u�W�F�N�g�ւ̃|�C���^
	ObjectBase* _parent;
	// �e�I�u�W�F�N�g�̃��f���n���h��
	int _parentModelHandle;

	
	std::vector<Afterglow*> _afterglowList;
};