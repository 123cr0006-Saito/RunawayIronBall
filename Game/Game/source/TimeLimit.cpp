//----------------------------------------------------------------------
// @filename TimeLimit.cpp
// ＠date: 2024/02/27
// ＠author: saito ko
// @explanation
// 制限時間の管理を行うクラス
//----------------------------------------------------------------------
#include "TimeLimit.h"
TimeLimit* TimeLimit::_instance = nullptr;
TimeLimit::TimeLimit() {
	// インスタンスを保存
	_instance = this;
	// 初期化
	_timeLimit = 0.0f;
	_startTime = 0.0f;
	_remainingTime = 0.0f;
	_setTime = 0;
	_IsStop = false;
	_stopTime = 0;
};

TimeLimit::~TimeLimit() {
	_instance = nullptr;
};

void TimeLimit::SetTimeLimit(int minutes, int second) {
	// 時間をミリ秒に変換
	_timeLimit = (minutes * 60 + second) * 1000;
	// 設定時間を保存
	_setTime = _timeLimit;
	// 開始時間を取得
	_startTime = GetNowCount();
	// 残り時間を計算
	_remainingTime = static_cast<float>(_timeLimit) - (GetNowCount() - _startTime); 
	// ミリ秒を秒に変換
	_remainingTime /= 1000;
};

void TimeLimit::Stop() {
	// ストップしていない場合
	if (!_IsStop) {
		_IsStop = true;
		_stopTime = GetNowCount();
	}
	// ストップしている場合
	else {
		// ストップ時間を更新
		_startTime += GetNowCount() - _stopTime;
		_stopTime = GetNowCount();
	}
};

void TimeLimit::Restart() {
	// 止まっていた時間を開始時間に足す
	_startTime += GetNowCount() - _stopTime;
	_stopTime = 0;
	_IsStop = false;
};

float TimeLimit::Process() {
	// ストップしている場合、止まる前の最後の時間を返す
	if (_IsStop) {
		return _remainingTime;
	}

	// 残り時間を計算
	_remainingTime = static_cast<float>(_timeLimit) - (GetNowCount() - _startTime); 
	// ミリ秒を秒に変換
	_remainingTime /= 1000; 
	return _remainingTime;
};

int TimeLimit::SecondsToTime() {
	// 残り時間を分と秒に変換
	float time = _remainingTime / 60;
	int min = static_cast<int>(time);
	int sec = (time - min) * 60;

	return min * pow(10.0f,2.0f) + sec;
};

int TimeLimit::GetElapsedSecond(){
	// 経過時間を取得
	float time = (GetNowCount() - _startTime) / (60 * 1000);
	
	int min = static_cast<int>(time);
	int sec = (time - min) * 60;

	return min * pow(10.0f, 2.0f) + sec;
};