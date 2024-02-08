#include "appframe.h"
#include "AnimationItem.h"

AnimationItem::AnimationItem()
{
	_attachIndex = -1;
	_totalTime = 0.0f;
	_playTime = 0.0f;
	_closeTotalTime = 0.0f;
	_closeTime = 0.0f;
	_loopCnt = 0;
}

AnimationItem::~AnimationItem()
{
}

void AnimationItem::Setup(int attachIndex, float totalTime, int loopTimes)
{
	_attachIndex = attachIndex;
	_totalTime = totalTime;
	_playTime = 0.0f;
	_closeTotalTime = 0.0f;
	_closeTime = 0.0f;
	_loopCnt = loopTimes;
}
