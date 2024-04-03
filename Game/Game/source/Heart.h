//----------------------------------------------------------------------
// @filename Heart.h
// ＠date: 2024/03/03
// ＠author: saito ko
// @explanation
// プレイヤーが触れたらＨＰを回復するオブジェクトクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class Heart
{
public :
	Heart(VECTOR pos);
	~Heart();
	bool Process();
	bool Render();

	Sphere GetSphere() { return _sphere; }
protected:
	int _model;
	int _currentTime;
	VECTOR _pos;
	Sphere _sphere;
};

