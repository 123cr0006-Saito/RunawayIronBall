#pragma once
#include "dxlib.h"
#include "player.h"
class Fog
{
	public:
	Fog();
	~Fog();
	void Process();
	void UpdateIsFog();
protected:
	Player* _player;
	bool IsFog;
	const int _fogDistanceMax = 30000;
	const int _fogDistanceMin = 2000;
	float _fogEndDistance;
	int _param;
	int _easingCount;
};

