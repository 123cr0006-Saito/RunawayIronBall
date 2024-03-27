#pragma once
#include "dxlib.h"
#include <math.h>
#include <string>
class Effect
{
public:
	Effect();
	~Effect();
	void SetVibration(float x, float y, float time);//画面振動
	void ScreenVibration();

protected:
	//振動用の変数
	float vibration_x,vibration_y;
	float max_x, max_y;
	float time;
	bool use_flag;
};