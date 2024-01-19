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

	VECTOR GetBallPosition() { return _iPos; }
	float GetBallRadius() { return _r; }

	void DrawDebugInfo();


private:
	XInput* _input;

	// çΩ
	int _cModelHandle;
	VECTOR _cPos[CHAIN_MAX];

	// ìSãÖ
	int _iModelHandle;
	VECTOR _iPos;
	VECTOR _iForwardDir;
	float _r = 130.0f; //// å„Ç≈SphereÉNÉâÉXÇçÏÇÈ



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