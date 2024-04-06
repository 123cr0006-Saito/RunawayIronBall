//----------------------------------------------------------------------
// @filename ScreenVibration.h
// ＠date: 2023/12/15
// ＠author: saito ko
// @explanation
// 画面上でのカメラが見ている映像の中心座標をずらす処理行うクラス
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
	void SetVibration(float x, float y, float time);//画面振動
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
	//振動用の変数
	float vibration_x,vibration_y;
	float max_x, max_y;
	float time;
	bool use_flag;
};