#pragma once
#include "dxlib.h"
#include <math.h>
#include <string>
class Effect
{
public:
	Effect();
	~Effect();
	void SetVibration(float x, float y, float time);//��ʐU��
	void ScreenVibration();

protected:
	//�U���p�̕ϐ�
	float vibration_x,vibration_y;
	float max_x, max_y;
	float time;
	bool use_flag;
};