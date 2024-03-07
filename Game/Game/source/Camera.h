#pragma once

#include "CameraBase.h"
class Camera : public CameraBase
{
public:
	Camera(VECTOR InitPos);
	~Camera();

	virtual bool Process(VECTOR pos, int map);

	float GetCamX() { return _cameraDirX; }//プレイヤーなどで使うかもしれないためゲッターとして作っておく
	float GetCamY() { return _cameraDirY; }//プレイヤーなどで使うかもしれないためゲッターとして作っておく

	void SetCameraDistance();
	bool ZoomProcess();
	
protected:
	static const int CAMERA_ZOOM_MAX = 3;

	int _reverseX;//カメラ操作の入力によるX方向を決める変数
	int _reverseY;//カメラ操作の入力によるY方向を決める変数

	bool _IsZoom;
	float _startDistance, _endDistance;

	int _cameraDistanceCount; // カメラの距離を3段階に分ける
	float _cameraChangeDistance[CAMERA_ZOOM_MAX];

	VECTOR _gazeShift;//注視点をプレイヤーにしたときに基準点が足元なので腰あたりに移動させるための変数
};