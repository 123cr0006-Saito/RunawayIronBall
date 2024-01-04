#include "Timer.h"
Timer::Timer() {
	_lastTime = 0;
	_elapsedTime = 0.0f;
}

bool Timer::Start() {
	_lastTime = GetNowHiPerformanceCount();
	return true;
}

double Timer::TimeElapsed() {
	LONGLONG _nowTime = GetNowHiPerformanceCount();
	int _interval = (int)(_nowTime - _lastTime);
	_elapsedTime = (double)_interval / 1000000;
	_lastTime = _nowTime;
	return _elapsedTime;//前フレームからの経過時間を返す
}