//----------------------------------------------------------------------
// @filename ModeZoomCamera.h
// ＠date: 2024/03/09
// ＠author: saito ko
// @explanation
// ゲート表示時にカメラを移動させるクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "TargetZoomCamera.h"
#include "Player.h"
class ModeZoomCamera : public ModeBase
{
	typedef ModeBase base;
public:
	ModeZoomCamera(VECTOR pos);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
protected:
	TargetZoomCamera* _camera;
	int _time;
	int _currentTime;
	VECTOR _pos;
};

