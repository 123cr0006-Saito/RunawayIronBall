#pragma once
#include "appframe.h"
#include "CharacterBase.h"

class Enemy : public CharacterBase
{
public:
	Enemy(int modelHandle, VECTOR pos);
	~Enemy();


	bool Process(VECTOR playerPos);

	bool GetIsHit() { return _isHit; }
	void SetIsHit(VECTOR dir) {
		_isHit = true;
		_cnt = 0;
		_blastOffDir = dir; 
		_blastOffPower = 120.0f;
	}

private:

	float _speed;

	VECTOR _startPos;


	bool _isHit;
	int _cnt;
	VECTOR _blastOffDir;
	float _blastOffPower;
};