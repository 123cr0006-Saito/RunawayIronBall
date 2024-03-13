#pragma once
#include "appframe.h"
#include "ObjectBase.h"

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
	~IronBall();

	void Init();
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

	Sphere GetCollision() { return _sphereCollision; }
	void UpdateCollision();



	// このオブジェクトを保有している親オブジェクト関連の関数
	// プレイヤーのモデルハンドルをセット
	void SetPlayerModelHandle(int handle);
	void SetMoveState(IB_MOVE_STATE state) { _moveState = state; }

	void SetParentInstance(ObjectBase* parent) { _parent = parent; }
	ObjectBase* GetParentInstance() { return _parent; }
	void SetParentPosPtr(VECTOR* pos) { _parentPos = pos; }
	VECTOR* GetParentPosPtr() { return _parentPos; }

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

	Sphere _sphereCollision;

	VECTOR _ibDefaultScale;

	// 配置ソケット
	int _socketNo[3];


	// このオブジェクトを保有している親のオブジェクトへのポインタ
	ObjectBase* _parent;
	// このオブジェクトを保有している親の座標へのポインタ
	VECTOR* _parentPos;


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

	//-------------------
	// 齋藤が作成した変数です。
	std::map<int, std::pair<int, float>> _powerAndScale;//攻撃力と拡大率を格納したコンテナです。
};