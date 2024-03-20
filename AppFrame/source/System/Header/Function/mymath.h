#pragma once
#include "dxlib.h"
#include <math.h>
#include "Easing.h"
class Math
{
public:
	// degree¨radian 
	static float DegToRad(float deg);

	// radian¨degree
	static float RadToDeg(float rad);

	static float Max(float left, float right);

	static float Min(float left, float right);

	static float Clamp(float min_n, float max_n, float n);

	//0:x 1:y 2:z
	static VECTOR NormRotMatrix(VECTOR norm, float rotation, int type);

	static VECTOR MatrixToVector(MATRIX matrix, int index);

	static float LenSegOnSeparateAxis(VECTOR Sep, VECTOR e1, VECTOR e2, VECTOR e3 = VGet(0, 0, 0));

	static MATRIX MMultXYZ(float dir_x, float dir_y, float dir_z);

	static MATRIX MMultXYZToMatrix(float dir_x, float dir_y, float dir_z);


	static bool  SetMatrixToVector(VECTOR& vec_x, VECTOR& vec_y, VECTOR& vec_z, const MATRIX matrix);

    //‰sŠp‚©‚Ç‚¤‚©
	static bool CheckAcuteAngle(VECTOR p1, VECTOR p2, VECTOR p3);

	//•½s‚©‚Ç‚¤‚©
	static bool CheckParallelRelation(VECTOR line_1_start, VECTOR line_1_end, VECTOR line_2_start, VECTOR line_2_end);

	//‚’¼‚©‚Ç‚¤‚©
	static bool CheckVerticalRelation(VECTOR line_1_start, VECTOR line_1_end, VECTOR line_2_start, VECTOR line_2_end);

	// 2‚Â‚ÌƒxƒNƒgƒ‹‚Ì‚È‚·Šp‚ğŒvZ‚·‚é
	// –ß‚è’l: Šp“x(radian) 
	static float CalcVectorAngle(VECTOR v1, VECTOR v2);

	// 3Dƒ‚ƒfƒ‹‚ğˆø”vForward‚Ì•ûŒü‚ª³–Ê‚É‚È‚é‚æ‚¤‚É‰ñ“]‚³‚¹‚é
	static void SetModelForward_RotationY(int modelHandle, VECTOR vForward);
};