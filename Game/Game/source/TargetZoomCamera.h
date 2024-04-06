//----------------------------------------------------------------------
// @filename TargetZoomCamera.h
// ＠date: 2024/03/09
// ＠author: saito ko
// @explanation
// 今のカメラの位置を指定の位置に移動する処理を行うクラス
//----------------------------------------------------------------------
#pragma once
#include "CameraBase.h"
#include "Camera.h"
class TargetZoomCamera : public CameraBase
{
public:
	TargetZoomCamera(VECTOR target, VECTOR nextPos, VECTOR keepDir,VECTOR nextDir, int time);
	~TargetZoomCamera();
	virtual bool Process()override;

protected:
	float _moveTime;//移動時間
	VECTOR _targetPos, _nextPos, _keepPos;// ターゲットの座標ともともとカメラがあった位置の座標
	VECTOR _nextDir, _keepDir; // 次のカメラの向きともともとのカメラの向き
};

