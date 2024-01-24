#pragma once
#include "dxlib.h"
#include <math.h>
#include <string>
class ScreenVibration
{
public:
	ScreenVibration();
	~ScreenVibration();
	void SetVibration(float x, float y, float time);//‰æ–ÊU“®
	void UpdateScreenVibration();
	
	static ScreenVibration* _instance;
	static ScreenVibration* GetInstance() { return _instance; }

#ifdef _DEBUG
	virtual float DispSizeW() { return 1280.0f; }
	virtual float DispSizeH() { return 720.0f; }
#else
	virtual float DispSizeW() { return 1920.0f; }
	virtual float DispSizeH() { return 1080.0f; }
#endif

protected:
	//U“®—p‚Ì•Ï”
	float vibration_x,vibration_y;
	float max_x, max_y;
	float time;
	bool use_flag;
};