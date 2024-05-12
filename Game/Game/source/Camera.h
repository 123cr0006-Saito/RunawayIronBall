//----------------------------------------------------------------------
// @filename Camera.h
// @date: 2023/12/14
// @author: saito ko
// @explanation
// プレイヤーを追従する基本的なカメラ
//----------------------------------------------------------------------
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
	void SetForwardCamera();
	void MoveProcess();//カメラの移動を行う関数
	
protected:
	static const int CAMERA_ZOOM_MAX = 3;//カメラのズーム段階

	int _reverseX;//カメラ操作の入力によるX方向を決める変数
	int _reverseY;//カメラ操作の入力によるY方向を決める変数

	bool _IsZoom;//ズーム中かどうか
	bool _IsForwardCamera;//カメラの向きを前方に向けるかどうか

	float _startDistance, _endDistance;//カメラの距離を変えるための変数
	float _startDirY, _endDirY;//カメラの向きを変えるための変数

	int _cameraDistanceCount; // カメラの距離を3段階に分ける
	float _cameraChangeDistance[CAMERA_ZOOM_MAX];//カメラの距離を変えた時の距離を格納する変数
	int _zoomCount;;//何番目のカメラの距離かのカウント
	int _forwardCount;//カメラの向きを変えるためのイージングカウント

	VECTOR _gazeShift;//注視点をプレイヤーにしたときに基準点が足元なので腰あたりに移動させるための変数
};