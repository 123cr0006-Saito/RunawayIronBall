//----------------------------------------------------------------------
// @filename Afterglow.cpp
// @date: 2024/12/14
// @author: saito ko
// @explanation
// それぞれのカメラクラスで共通で使用する変数がある基底クラス
//----------------------------------------------------------------------
#pragma once
#include "dxlib.h"
#include "appframe.h"
class CameraBase
{
public:
	CameraBase();
	~CameraBase();
	virtual bool Process();

    //カメラの処理を行う関数
	float GetTargetDistance() { return _pointDistance.z; }//プレイヤーなどで使うかもしれないためゲッターとして作っておく
	static float GetMaxLength() { return _cameraMaxDistance - _cameraMinDistance; }

protected:
	static const float _cameraMinDistance;//カメラの最小距離
	static const float _cameraMaxDistance;//カメラの最大距離
	XInput* _input;//コントローラーでの入力を受け付けるためのinputクラス

	float _cameraDirX;//カメラのＸ軸回転行列で使用する変数
	float _cameraDirY;//カメラのＹ軸回転行列で使用する変数
	XInput::STICK _stick;//コントローラーのスティックの入力を受け付けるための変数

	VECTOR _pointDistance;//注視点からの距離
	int _currentTime;//現在の時間
};

