//----------------------------------------------------------------------
// @filename CrystarPattern2.h
// @date: 2024/01/26
// @author: saito ko
// @explanation
// クライスターのパターン2(岩)のクラス 頭突き攻撃をします。
//----------------------------------------------------------------------
#pragma once
#include "EnemyBase.h"
#include "CrystarRoof.h"
class CrystarPattern2 :public EnemyBase
{
public:
	CrystarPattern2();
	~CrystarPattern2()override;

	void Init(VECTOR pos)override;
	void InheritanceInit()override;
	void AnimInit()override;
	void CommandProcess()override;

	bool ModeSearch(bool plAttack)override;
	bool ModeDisCover()override;
	bool ModeAttack()override;
	bool ModeCoolTime()override;
	bool ModeKnockBack()override;

	bool IndividualProcessing()override;
	bool IndividualRendering()override;
	bool SetState()override;

	bool DebugRender()override;
	VECTOR GetCollisionPos()override { return MV1GetFramePosition(_model, _collisionFrame); }

	int GetModelHandle() { return _model; }
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
	CrystarRoof* _roof;
};

