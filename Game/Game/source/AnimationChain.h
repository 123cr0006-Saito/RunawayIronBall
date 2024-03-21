#pragma once
#include <math.h>
#include "../../Appframe/source/System/Header/Function/Easing.h"
#include "dxlib.h"
class AnimationChain
{
public : 
	AnimationChain(VECTOR pos, float angle);
	~AnimationChain();
	void Process();
	void Draw();
protected:
	int _handle;
	int _frameCount;
	int _maxCount;
	float _angle;
	float _startX,_startY,_endX,_endY;
	float _nowX,_nowY;
};

