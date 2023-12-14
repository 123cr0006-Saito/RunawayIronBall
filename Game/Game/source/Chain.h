#pragma once
#include "appframe.h"


#define CHAIN_MAX 12

class Chain {
public:

	void Init();
	void Process(VECTOR playerPos);
	void AnimProcess();

	void Render();

	void DrawDebugInfo();


private:
	XInput* _input;

	int _modelHandle[CHAIN_MAX];

	VECTOR _pos[CHAIN_MAX];
	VECTOR _mbDir;



	int _attackCnt;


	int _animIndex;
	float _animTotalTime;
	float _playTime;


	float _cnt;

	MATRIX _m[CHAIN_MAX];

	float _length;

	bool _isSwing;
};