#include "Camera.h"
Camera::Camera(XInput* input) {
	this->input = input;
	
	cam_gaze_shift = VGet(0, 200, 0);
	move_camera = nullptr;
	 keep_dir = 0.0f;
	 next_dir = 0.0f;
	 flag = false;
	 count = 0;
	 cam_len = VGet(0, 300, -400);
};

Camera::~Camera() {
	input = nullptr;
};

bool Camera::Input() {
	return true;
};

bool Camera::Process(VECTOR pos) {

	auto move_speed_process = [](float pos, float pos_max, float max_speed) {return pos * max_speed / pos_max; };
	
	//if (input->GetRx() != 0) {
	//	camera_dir_y += move_speed_process(input->GetRx(), 32768, 0.02);
	//}
	//if (input->GetRy() != 0) {
	//	camera_dir_x += move_speed_process(input->GetRy(), 32768, 0.02);
	//}


	//if (input->GetKey(XINPUT_BUTTON_DPAD_DOWN)) {
	//	camera_dir_y -= 0.02;
	//}
	//if (input->GetKey(XINPUT_BUTTON_DPAD_UP)) {
	//	camera_dir_y += 0.02;
	//}
	//if (input->GetKey(XINPUT_BUTTON_DPAD_LEFT)) {
	//	camera_dir_x -= 0.02;
	//}
	//if (input->GetKey(XINPUT_BUTTON_DPAD_RIGHT)) {
	//	camera_dir_x += 0.02;
	//}

	if (input->GetRTrg() > 25) {
		if (cam_len.z > -800) {
			cam_len.z -= input->GetRTrg() / 25;
		}
	}

	if (input->GetLTrg() > 25) {
		if (cam_len.z < -200) {
			cam_len.z += input->GetLTrg() / 25;
		}
	}


	if (abs(Math::RadToDeg(camera_dir_x)) > 360) {
		camera_dir_x = 0;
	}
	if (abs(Math::RadToDeg(camera_dir_y)) > 360) {
		camera_dir_y = 0;
	}

	if (flag) {
		if (count < 90) {
			*move_camera = Easing::Linear(count, keep_dir, next_dir, 90);
			count++;
		}
		else {
			move_camera = nullptr;
			flag = false;
		}
	}


	MATRIX moto = MGetIdent();
	MATRIX Matrix = MGetRotY(camera_dir_y);
	MATRIX Matrix2 = MGetRotX(camera_dir_x);

	moto = MMult(moto, Matrix2);
	moto = MMult(moto, Matrix);

	VECTOR Vecter = VTransform(cam_len,moto);

	VECTOR VecAdd = VAdd(Vecter, pos);

	int shift_y = 0;
	//if (VecAdd.y <= 0) {
	//	shift_y = VecAdd.y;
	//	VecAdd.y = 0;
	//}

	SetCameraPositionAndTarget_UpVecY(VecAdd, VAdd(pos, VSub(cam_gaze_shift,VGet(0,shift_y,0))));

	return true;
};

void Camera::SetCamera(float dir,int shaft) {
	if (!flag) {
		switch (shaft) {
		case 0:
			move_camera = &camera_dir_x;
			keep_dir = camera_dir_x;
			break;
		case 1:
			move_camera = &camera_dir_y;
			keep_dir = camera_dir_y;
			break;
		}
		next_dir = dir;
		count = 0;
		flag = true;
	}
};

bool Camera::DebugDraw(VECTOR pos) {
	clsDx();
	printfDx("%d", flag);
	return true;
};
