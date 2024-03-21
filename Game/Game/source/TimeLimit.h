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
};

