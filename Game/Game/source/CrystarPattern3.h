//----------------------------------------------------------------------
// @filename CrystarPattern3.h
// ＠date: 2024/01/26
// ＠author: saito ko
// @explanation
// 一番強いクライスターのパターン3(鉄)のクラス 回転しながら追尾攻撃をします。
//----------------------------------------------------------------------
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

	VECTOR _attackPos;//攻撃時に増える値
	float _attackDir;//攻撃した時に使用する角度

	CrystarRoof* _roof[2];
};

