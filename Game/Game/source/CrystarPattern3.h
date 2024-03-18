#pragma once
#include "EnemyBase.h"
#include "CrystarRoof.h"

class CrystarPattern3 : public EnemyBase
{
public:
	CrystarPattern3();
	~CrystarPattern3()override;

	void Init(VECTOR pos)override;
	void InheritanceInit()override;
	void AnimInit()override;
	void CommandProcess()override;

	bool ModeSearch(bool plAttack)override;
	bool ModeAttack()override;
	bool ModeCoolTime()override;
	bool ModeKnockBack()override;

	bool IndividualProcessing()override;
	bool IndividualRendering()override;
	bool SetState()override;

	bool DebugRender()override;
	VECTOR GetCollisionPos()override { return MV1GetFramePosition(_modelHandle, _collisionFrame); }

	int GetModelHandle() { return _modelHandle; }
protected:

	static enum ANIMSTATE : int {
		IDLE = 0,
		WALK,
		RUN,
		HANDSTAND,
		HANDSTANDLOOP,
		HANDBUTT
	};
	static int _collisionFrame;
	ANIMSTATE _animState;

	VECTOR _attackPos;//çUåÇéûÇ…ëùÇ¶ÇÈíl
	float _attackDir;//çUåÇÇµÇΩéûÇ…égópÇ∑ÇÈäpìx

	CrystarRoof* _roof[2];
};

