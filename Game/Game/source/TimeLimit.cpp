#include "TimeLimit.h"
TimeLimit* TimeLimit::_instance = nullptr;
TimeLimit::TimeLimit() {
	_instance = this;
	_timeLimit = 0;
	_startTime = 0;
	_remainingTime = 0.0f;
};

TimeLimit::~TimeLimit() {
	_instance = nullptr;
};

void TimeLimit::SetTimeLimit(int minutes, int second) {
	_timeLimit = (minutes * 60 + second) * 1000;
	_startTime = GetNowCount();
};

float TimeLimit::Process() {
	_remainingTime = static_cast<float>(_timeLimit) - (GetNowCount() - _startTime); // Žc‚èŽžŠÔ‚ðŒvŽZ
	_remainingTime /= 1000; // ƒ~ƒŠ•b‚ð•b‚É•ÏŠ·
	return _remainingTime;
};

int TimeLimit::SecondsToTime() {
	float time = _remainingTime / 60;
	int min = static_cast<int>(time);
	int sec = (time - min) * 60;

	return min * pow(10.0f,2.0f) + sec;
};