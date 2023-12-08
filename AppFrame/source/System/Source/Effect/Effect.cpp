#include "../../Header/Effect/Effect.h"

Effect::Effect() {
	vibration_x = 0.0f;
	vibration_y = 0.0f;
	max_x = 0.0f;
	max_y = 0.0f;
	time = 0.0f;
	use_flag = false;
};

Effect::~Effect() {

};

void Effect::SetVibration(float x, float y, float time) {
	vibration_x = x;
	vibration_y = y;
	max_x = x;
	max_y = y;
	this->time = time;
};

void Effect::ScreenVibration() {
	if (use_flag) {
		//zŽ²‚É‘Î‚µ‚Ä
		max_x -= max_x / time;
		max_y -= max_y / time;
		if (max_y <= 0) {
			if (max_x <= 0) {
				vibration_x = 0;
				vibration_y = 0;
				use_flag = false;
			}
		}
		else {
			vibration_x = rand() % (int)ceil(max_x) - ceil(max_x / 2);
			vibration_y = rand() % (int)ceil(max_y) - ceil(max_y / 2);
		}
		SetCameraScreenCenter((Effect::DispSizeW() / 2) + vibration_x, (Effect::DispSizeH() / 2) + vibration_y);
	}
};

//void Effect::DrawBillBoard(VECTOR center_pos, VECTOR shift_pos, int handle,float angle) {
//	VECTOR cam = VTransform(shift_pos, MGetRotY(camera->GetCamY()));
//	DrawBillboard3D(VAdd(center_pos, cam), 0.5f, 0.5f, 500.0f, angle,handle, TRUE);
//};
