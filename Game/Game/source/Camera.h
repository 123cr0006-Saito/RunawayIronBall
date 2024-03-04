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

	
protected:

	int _reverseX;//カメラ操作の入力によるX方向を決める変数
	int _reverseY;//カメラ操作の入力によるY方向を決める変数

	VECTOR _gazeShift;//注視点をプレイヤーにしたときに基準点が足元なので腰あたりに移動させるための変数
};