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
	static int _collisionFrame;
	static enum ANIMSTATE : int {
		IDLE = 0,
		WALK,
		RUN,
		HANDSTAND,
		HANDSTANDLOOP,
		HANDBUTT
	};
	ANIMSTATE _animState;
	CrystarRoof* _roof;
};

