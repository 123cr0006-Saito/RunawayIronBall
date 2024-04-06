//----------------------------------------------------------------------
// @filename IronBall.h
// ＠date: 2024/04/01
// ＠author: Morozumi Hiroya
// @explanation
// プレイヤーが持つ鉄球・鎖の制御・描画を行うクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "ObjectBase.h"
#include "Afterglow.h"

#define CHAIN_MAX 12

enum IB_MOVE_STATE {
	FOLLOWING,
	PUTTING_ON_SOCKET,
	INTERPOLATION,
};
class IronBall : public ObjectBase
{
public:
	IronBall();
	virtual ~IronBall();

	void Init();
	// 親モデルの座標をもとに初期位置を設定
	void InitPosFromParent();
	void Process();

	void MoveProcess();
	void FollowingProcess();
	void PuttingOnSocketProcess();
	void InterpolationProcess();

	void AnimProcess();

	void Render();

	VECTOR GetBallPosition() { return _iPos; }
	void SetBallPosition(VECTOR pos) { _iPos = pos; }

	VECTOR* GetBallPosPtr() { return &_iPos; }

	bool GetEnabledAttackCollision() { return _enabledAttackCollision; }
	void SetEnabledAttackCollision(bool state) { _enabledAttackCollision = state; }

	Sphere GetIBCollision() { return _ibAttackSphereCollision; }
	Sphere GetIBBodyCollision() { return _ibBodySphereCollision; }
	Capsule GetChainCollision() { return _chainCapsuleCollision; }
	void UpdateIBCollision();
	void UpdateChainCollision();
	// サイトウが作成しました。
	void SetEnabledAfterGlow(bool enable);// 目の残光を表示するかどうかを設定

	// このオブジェクトを保有している親オブジェクト関連の関数
	// プレイヤーのモデルハンドルをセット
	void SetPlayerModelHandle(int handle);
	void SetMoveState(IB_MOVE_STATE state) { _moveState = state; }

	void SetParentInstance(ObjectBase* parent) { _parent = parent; }
	ObjectBase* GetParentInstance() { return _parent; }

	// デバッグ情報の表示
	void DrawDebugInfo();


	bool UpdateLevel(float scale);//プレイヤーから取得した、レベルで攻撃力と拡大率を設定


private:
	XInput* _input;

	// 鎖
	int _cModelHandle;
	VECTOR _cPos[CHAIN_MAX];

	// 鉄球
	int _iModelHandle;
	VECTOR _iPos;
	VECTOR _iForwardDir;

	// 鉄球部分の当たり判定形状（地面との当たり判定に使う）
	Sphere _ibBodySphereCollision;
	// 鉄球部分の攻撃当たり判定
	Sphere _ibAttackSphereCollision;
	// 鎖部分の当たり判定
	Capsule _chainCapsuleCollision;
	// 鎖部分の当たり判定をCollisionManagerに登録するためのCell
	Cell* _chainCell;

	VECTOR _ibDefaultScale;

	// 配置ソケット
	int _socketNo[3];


	// このオブジェクトを保有している親のオブジェクトへのポインタ
	ObjectBase* _parent;

	int _attackAnimCnt;


	int _animIndex;
	float _animTotalTime;
	float _playTime;


	float _cnt;
	int _attackDir;

	MATRIX _m[CHAIN_MAX];

	float _length;

	bool _followingMode;
	
	IB_MOVE_STATE _moveState;
	bool _enabledAttackCollision;


	int _playerModelHandle;

	std::vector<Afterglow*> _afterglowList;
};