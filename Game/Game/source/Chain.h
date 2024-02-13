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

	void DrawDebugInfo();

	//齋藤が作成した関数です------------------------
	void SetPowerScale(std::string FileName);//ファイル読み込みでレベルに合わせた攻撃力と拡大率を取得
	bool UpdateLevel();//プレイヤーから取得した、レベルで攻撃力と拡大率を設定
	int GetPower() { return _power; }//ノックバック用の力を返します。
	//----------------------------------------------------------


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


	Player* _playerInstance;
	int _playerModelHandle;

	//-------------------
	// 齋藤が作成した変数です。
	const int _originR = 50;
	int _power;//吹っ飛ばす力です。
	std::map<int, std::pair<int, float>> _powerAndScale;//攻撃力と拡大率を格納したコンテナです。
};