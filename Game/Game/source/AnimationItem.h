#pragma once
#include "appframe.h"

class AnimationItem
{
public:
	AnimationItem();
	~AnimationItem();

private:
	int _attachIndex;
	float _totalTime;
	float _playTime;
	// モーションブレンド用
	float _closeTotalTime;
	float _closeTime;
	// ループカウント用
	int _loopCnt;
};