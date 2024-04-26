//----------------------------------------------------------------------
// @filename ScreenVibration.h
// @date: 2023/12/15
// @author: saito ko
// @explanation
// ‰æ–Êã‚Å‚ÌƒJƒƒ‰‚ªŒ©‚Ä‚¢‚é‰f‘œ‚Ì’†SÀ•W‚ğ‚¸‚ç‚·ˆ—s‚¤ƒNƒ‰ƒX
//----------------------------------------------------------------------
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
	void SetVibrationX(float x, float time);
	void SetVibrationY(float y, float time);
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
	float vibration_x,vibration_y;//U“®—Ê
	float max_x, max_y;//U“®‚ÌÅ‘å’l
	float time;//U“®ŠÔ
	bool use_flag;//U“®’†‚©‚Ç‚¤‚©
};