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
	// ���[�V�����u�����h�p
	float _closeTotalTime;
	float _closeTime;
	// ���[�v�J�E���g�p
	int _loopCnt;
};