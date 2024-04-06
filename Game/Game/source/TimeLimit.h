//----------------------------------------------------------------------
// @filename TimeLimit.h
// —date: 2024/02/27
// —author: saito ko
// @explanation
// §ŒÀŠÔ‚ÌŠÇ—‚ğs‚¤ƒNƒ‰ƒX
//----------------------------------------------------------------------
#pragma once
#include "dxlib.h"
#include <math.h>
class TimeLimit
{
	public:
	TimeLimit();
	~TimeLimit();
	void SetTimeLimit(int minutes, int second);
	
	float Process();
	int SecondsToTime();
	void Stop();
	void Restart();
	
	static TimeLimit* _instance;
	static TimeLimit* GetInstance() { return _instance; }

	float GetRemainingTime() { return _remainingTime; }
	int GetTimeLimit() { return SecondsToTime(); }
	int GetElapsedTime() { return GetElapsedSecond(); }
	int GetElapsedSecond();

	int GetStartTime() { return _setTime / 1000; }

protected:
	float _startTime,_timeLimit;
	float _setTime;
	float _remainingTime;
	int _stopTime;
	bool _IsStop;
};

