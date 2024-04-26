//----------------------------------------------------------------------
// @filename TimeLimit.h
// @date: 2024/02/27
// @author: saito ko
// @explanation
// 制限時間の管理を行うクラス
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
	float _startTime,_timeLimit;// 開始時間と制限時間
	float _setTime;// 設定時間
	float _remainingTime;// 残り時間
	int _stopTime;// ストップした時間
	bool _IsStop;// 時間を止めるかどうか
};

