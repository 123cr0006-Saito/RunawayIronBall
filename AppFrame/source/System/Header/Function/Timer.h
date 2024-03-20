#pragma once
#include "dxlib.h"
class Timer
{
public:
	Timer();
	bool Start();//このクラスを呼び出したときに宣言する
	double TimeElapsed();//前のフレームからの経過時間を計算

	//--仮
	LONGLONG GetLastTime() { return _lastTime; }
	double GetElapsedTime() { return _elapsedTime; }
private:
	LONGLONG _lastTime;//前のフレームの最後の時間
	double _elapsedTime;//前のフレームからの経過時間
};

