#pragma once
#include "dxlib.h"
#include <math.h>
class TDD
{
public:
	static bool ThrDimPosDraw(VECTOR pos);
	static bool TweDimColDraw(VECTOR pos, float w, float d);
	static bool ThrDimColDraw(VECTOR pos,float w, float h, float d, float direction);
	static bool ThrDimColToBillBoradDraw(VECTOR pos, float wide, float height, float depth,float direction_z);
	static bool TweDimColOfCircleDraw(VECTOR pos,float r);
	static bool ThrDimColOfCircleDraw(VECTOR pos, float r,float h,bool flag);
};

