//----------------------------------------------------------------------
// @filename ScreenVibration.cpp
// @date: 2023/12/15
// @author: saito ko
// @explanation
// 画面上でのカメラが見ている映像の中心座標をずらす処理行うクラス
//----------------------------------------------------------------------
#include "ScreenVibration.h"

ScreenVibration* ScreenVibration::_instance = nullptr;
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return 無し
//----------------------------------------------------------------------
ScreenVibration::ScreenVibration() {
	_instance = this;
	vibration_x = 0.0f;
	vibration_y = 0.0f;
	max_x = 0.0f;
	max_y = 0.0f;
	time = 0.0f;
	use_flag = false;
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
ScreenVibration::~ScreenVibration() {
	_instance = nullptr;
};
//----------------------------------------------------------------------
// @brief 揺れる最大の移動値と時間を設定
// @param x x軸の最大移動値
// @param y y軸の最大移動値
// @param time 揺れる時間
// @return 無し
//----------------------------------------------------------------------
void ScreenVibration::SetVibration(float x, float y, float time) {
		vibration_x = x;
		vibration_y = y;
		max_x = x;
		max_y = y;
		this->time = time;
		use_flag = true;
};
//----------------------------------------------------------------------
// @brief x軸の最大移動値と時間を設定
// @param x x軸の最大移動値
// @param time 揺れる時間
// @return 無し
//----------------------------------------------------------------------
void ScreenVibration::SetVibrationX(float x, float time){
	vibration_x = x;
	max_x = x;
	this->time = time;
	use_flag = true;
};
//----------------------------------------------------------------------
// @brief y軸の最大移動値と時間を設定
// @param y y軸の最大移動値
// @param time 揺れる時間
// @return 無し
//----------------------------------------------------------------------
void ScreenVibration::SetVibrationY(float y, float time){
	vibration_y = y;
	max_y = y;
	this->time = time;
	use_flag = true;
};
//----------------------------------------------------------------------
// @brief 画面を揺らす処理
// @return 無し
//----------------------------------------------------------------------
void ScreenVibration::UpdateScreenVibration() {
	if (use_flag) {
		//z軸に対して
		if (max_y <= 0) {
			if (max_x <= 0) {
				vibration_x = 0;
				vibration_y = 0;
				use_flag = false;
			}
		}

		{
			if (max_x != 0) {
				max_x -= max_x / time;
				vibration_x = rand() % (int)ceil(max_x) - ceil(max_x / 2);
			}
			if (max_y != 0) {
				max_y -= max_y / time;
				vibration_y = rand() % (int)ceil(max_y) - ceil(max_y / 2);
			}
		}
		SetCameraScreenCenter((ScreenVibration::DispSizeW() / 2) + vibration_x, (ScreenVibration::DispSizeH() / 2) + vibration_y);
	}
};