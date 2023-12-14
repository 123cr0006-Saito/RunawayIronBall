#pragma once
#include "dxlib.h"
#include "appframe.h"
class Camera
{
public:
	Camera();
	~Camera();

	virtual bool Process(VECTOR pos);

	float GetCamX() { return _cameraDirX; }//プレイヤーなどで使うかもしれないためゲッターとして作っておく
	float GetCamY() { return _cameraDirY; }//プレイヤーなどで使うかもしれないためゲッターとして作っておく


protected:
	XInput* _input;//コントローラーでの入力を受け付けるためのinputクラス

	float _cameraDirX;//カメラのＸ軸回転行列で使用する変数
	float _cameraDirY;//カメラのＹ軸回転行列で使用する変数

	int _reverseX;//カメラ操作の入力によるX方向を決める変数
	int _reverseY;//カメラ操作の入力によるY方向を決める変数

	VECTOR _pointDistance;//注視点からの距離

	VECTOR _gazeShift;//注視点をプレイヤーにしたときに基準点が足元なので腰あたりに移動させるための変数

};