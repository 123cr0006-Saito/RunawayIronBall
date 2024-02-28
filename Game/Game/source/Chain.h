#pragma once
#include "appframe.h"
#include "Player.h"

#define CHAIN_MAX 12

class Chain {
public:

	void Init();
	void Process();

	void MoveProcess();
	void FollowingProcess();
	void PuttingOnSocketProcess();
	void InterpolationProcess();

	void AnimProcess();

	void Render();

	VECTOR GetBallPosition() { return _iPos; }
	float GetBallRadius() { return _r; }

	VECTOR* GetBallPosPtr() { return &_iPos; }

	bool GetEnabledAttackCollision() { return _enabledAttackCollision; }
	void SetEnabledAttackCollision(bool state) { _enabledAttackCollision = state; }

	Sphere GetCollision() { return _sphereCollision; }
	void UpdateCollision();

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
	float _r = 55.0f; //// 後でSphereクラスを作る

	Sphere _sphereCollision;

	VECTOR _ibDefaultScale;

	// 配置ソケット
	int _socketNo[3];


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


	Player* _playerInstance;
	int _playerModelHandle;

	//-------------------
	// 齋藤が作成した変数です。
	std::map<int, std::pair<int, float>> _powerAndScale;//攻撃力と拡大率を格納したコンテナです。
};