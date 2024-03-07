#pragma once
#include "EnemyBase.h"
class Chainguard : public EnemyBase
{
public:
	Chainguard();
	~Chainguard()override;
	void InheritanceInit()override;  //�p�����p�̏�����
	void AnimInit()override; // �A�j���[�V�����̏�����
	void CommandProcess()override;// �R�}���h���������ꍇ����

	bool ModeSearch()override; // �T�[�`���
	bool ModeAttack()override; // �U�����
	bool ModeCoolTime()override; // �N�[���^�C�����
	bool ModeKnockBack()override; // �m�b�N�o�b�N���

	bool SetGravity()override; // �d�͏����@�ύX����Ǝv���̂ł���܂����g�p���Ȃ������ꍇ�폜���Ă����v�ł��B
	void SetKnockBackAndDamage(VECTOR vDir, float damage)override;// �m�b�N�o�b�N�̏����@�m�b�N�o�b�N�����Ȃ���ԂɂȂ��Ă���̂Ń_���[�W�̏����ł��B

	bool IndividualProcessing()override; // �p�����p�̃v���Z�X�ł��B �Ƃ肠�����A�j���[�V�����̏��������Ă��܂��B
	bool IndividualRendering()override; // �p�����p�̕`��֐��ł��B �S���Ȃǂ���Ǝv���̂Ŏg�p���Ă�������

protected:
	static enum ANIMSTATE : int {
		ROTATE_START = 0,
		ROTATE_LOOP,
		ROTATE_END,
		WALK,
		IDLE,
		SWING,
		SWING_STIFF,
		SWING_BACK,
		REPALLED,
		STAN,
		WAKE_UP
	};

	ANIMSTATE _animState;
};