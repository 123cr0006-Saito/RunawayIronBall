#pragma once
#include "dxlib.h"
#include "appframe.h"
class Camera
{
public:
	Camera(XInput* input);
	~Camera();

	virtual bool Input();
	virtual bool Process(VECTOR pos);
	void SetCamera(float dir, int shaft);
	bool GetFlag() { return flag; }

	bool DebugDraw(VECTOR pos);

	float GetCamX() { return camera_dir_x; }
	float GetCamY() { return camera_dir_y; }


protected:
	XInput* input;

	float camera_dir_x = 0.0f;
	float camera_dir_y = 0.0f;;
	

	float* move_camera;
	float keep_dir;
	float next_dir;
	bool flag;
	int count;

	VECTOR cam_len;

	VECTOR cam_gaze_shift;//‘«Œ³‚ðŒ©‚é‚©‚ç‚¨‚È‚©‚ ‚½‚è‚É‚¸‚ç‚·

};

