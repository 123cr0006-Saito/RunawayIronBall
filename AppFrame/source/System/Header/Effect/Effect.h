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
	//void DrawBillBoard(VECTOR center_pos, VECTOR shift_pos, int handle,float angle);


#ifdef _DEBUG
	virtual float DispSizeW() { return 1280.0f; }
	virtual float DispSizeH() { return 720.0f; }
#else
	virtual float DispSizeW() { return 1920.0f; }
	virtual float DispSizeH() { return 1080.0f; }
#endif

protected:
	//振動用の変数
	float vibration_x,vibration_y;
	float max_x, max_y;
	float time;
	bool use_flag;
};