//----------------------------------------------------------------------
// @filename ModeRotationCamera.cpp
// ＠date: 2024/03/09
// ＠author: saito ko
// @explanation
// ステージ開始時のカメラの回転処理を行うクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "RotationCamera.h"
#include "ModeFadeComeBack.h"
class ModeRotationCamera : public ModeBase
{
	typedef ModeBase base;
public:
	ModeRotationCamera(int stageNum);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
protected:
	XInput* _input;// 入力クラス
	int _handle;// ステージの名前の画像ハンドル
	int _stageNum;// ステージの番号
	RotationCamera* _camera;// カメラクラス
	int _currentTime;// 現在の時間
};

