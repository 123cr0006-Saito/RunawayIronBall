//----------------------------------------------------------------------
// @filename CrystarPattern1.h
// @date: 2024/01/15
// @author: saito ko
// @explanation
// 一番弱いクライスターのパターン1(ガラス)のクラス 突進攻撃をします
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
	static int _collisionFrame;//当たり判定のフレーム番号
	static enum ANIMSTATE : int {
		IDLE = 0,
		WALK,
		RUN,
		HANDSTAND,
		HANDSTANDLOOP,
		HANDBUTT
	};
	ANIMSTATE _animState;//アニメーションの状態
	CrystarRoof* _roof;//クライスターの屋根
};

