#include "TimeLimit.h"
TimeLimit* TimeLimit::_instance = nullptr;
TimeLimit::TimeLimit() {
	_instance = this;
	_timeLimit = 0;
	_startTime = 0;
	_remainingTime = 0.0f;
};

TimeLimit::~TimeLimit() {
	// 特になし
};

void TimeLimit::SetTimeLimit(int minutes, int second) {
	_timeLimit = (minutes * 60 + second) * 1000;
	_startTime = GetNowCount();
};

float TimeLimit::Process() {
	_remainingTime = static_cast<float>(_timeLimit) - (GetNowCount() - _startTime); // 残り時間を計算
	_remainingTime /= 1000; // ミリ秒を秒に変換
	return _remainingTime;
};

int TimeLimit::SecondsToTime() {
	float time = _remainingTime / 60;
	int min = static_cast<int>(time);
	int sec = (time - min) * 60;

	return min * pow(10.0f,2.0f) + sec;
};