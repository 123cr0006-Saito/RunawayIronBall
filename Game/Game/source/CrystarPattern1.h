//----------------------------------------------------------------------
// @filename CrystarPattern1.h
// @date: 2024/01/15
// @author: saito ko
// @explanation
// ��Ԏア�N���C�X�^�[�̃p�^�[��1(�K���X)�̃N���X �ːi�U�������܂�
//----------------------------------------------------------------------
#pragma once
#include "EnemyBase.h"
#include "CrystarRoof.h"
class CrystarPattern1 :public EnemyBase
{
public:
	CrystarPattern1();
	~CrystarPattern1()override;

	void Init(VECTOR pos)override;
	void InheritanceInit()override;
	void AnimInit()override;
	void CommandProcess()override;

	bool ModeSearch(bool plAttack)override;
	bool ModeDisCover()override;
	bool ModeCoolTime()override;
	bool ModeKnockBack()override;

	bool IndividualProcessing()override;
	bool IndividualRendering()override;
	bool SetState()override;

	bool DebugRender()override;
	VECTOR GetCollisionPos()override { return MV1GetFramePosition(_model, _collisionFrame); }

	int GetModelHandle() { return _model; }
protected:
	static int _collisionFrame;//�����蔻��̃t���[���ԍ�
	static enum ANIMSTATE : int {
		IDLE = 0,
		WALK,
		RUN,
		HANDSTAND,
		HANDSTANDLOOP,
		HANDBUTT
	};
	ANIMSTATE _animState;//�A�j���[�V�����̏��
	CrystarRoof* _roof;//�N���C�X�^�[�̉���
};

