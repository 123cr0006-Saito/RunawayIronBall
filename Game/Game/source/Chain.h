#pragma once
#include "appframe.h"
#include "Player.h"

#define CHAIN_MAX 12

class Chain {
public:

	void Init();
	void Process(VECTOR playerPos);
	void AnimProcess();

	void Render();

	VECTOR GetBallPosition() { return _cPos[CHAIN_MAX - 1]; }

	void DrawDebugInfo();


private:
	XInput* _input;

	// ��
	int _cModelHandle;
	VECTOR _cPos[CHAIN_MAX];

	// �S��
	int _iModelHandle;
	VECTOR _iPos;

	VECTOR _iForwardDir;



	int _attackAnimCnt;


	int _animIndex;
	float _animTotalTime;
	float _playTime;


	float _cnt;
	int _attackDir;

	MATRIX _m[CHAIN_MAX];

	float _length;

	bool _isSwing;


	Player* _playerInstance;
	int _playerModelHandle;
};