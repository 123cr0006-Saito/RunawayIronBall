#include "ScreenVibration.h"

ScreenVibration* ScreenVibration::_instance = nullptr;

ScreenVibration::ScreenVibration() {
	_instance = this;
	vibration_x = 0.0f;
	vibration_y = 0.0f;
	max_x = 0.0f;
	max_y = 0.0f;
	time = 0.0f;
	use_flag = false;
};

ScreenVibration::~ScreenVibration() {

};

void ScreenVibration::SetVibration(float x, float y, float time) {
		vibration_x = x;
		vibration_y = y;
		max_x = x;
		max_y = y;
		this->time = time;
		use_flag = true;
};

void ScreenVibration::UpdateScreenVibration() {
	if (use_flag) {
		//zŽ²‚É‘Î‚µ‚Ä
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
