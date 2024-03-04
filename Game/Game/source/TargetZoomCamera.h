#pragma once
#include "CameraBase.h"
#include "Camera.h"
class TargetZoomCamera : public CameraBase
{
public:
	TargetZoomCamera();
	TargetZoomCamera(VECTOR pos, int time, Camera* camera);
	~TargetZoomCamera();

	virtual bool UpdateCameraToEvent();
	void SetEventCamera(VECTOR pos, int time);
	void SetReturnCamera(VECTOR pos);
	
protected:
	bool _IsEvent;//イベントなどで注視点がプレイヤー以外になるときに使用
	bool _IsReturn;//イベントフラグが立っているときにカメラをもとの座標に戻すのに使用

	int _moveTime;//移動時間
	VECTOR _targetPos, _keepPos;// ターゲットの座標ともともとカメラがあった位置の座標
};

