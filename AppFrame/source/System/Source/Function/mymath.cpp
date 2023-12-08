#include "../../Header/Function/mymath.h"
 //degreeÅ®radian
float Math::DegToRad(float deg) {
	return deg * PI / 180.0f;		// äpìxÇdegreeÇ©ÇÁradianÇ…ïœä∑
}

// radianÅ®degree
float Math::RadToDeg(float rad) {
	return rad * 180.0f / PI;		// äpìxÇradianÇ©ÇÁdegreeÇ…ïœä∑
}

//ÉuÉâÉìÉN
float Math::Max(float left, float right) {
	return left > right ? left : right;
};

float Math::Min(float left, float right) {
	return left < right ? left : right;
};

float Math::Clamp(float min_n, float max_n, float n) {
	if (n <= min_n) { return min_n; }
	if (n >= max_n) { return max_n; }
	return n;
}

//É}ÉgÉäÉNÉXÇçÏÇËÉxÉNÉgÉãÇê≥ãKâªÇµÇƒìnÇ∑
//ÉãÅ[Évï∂Ç≈égÇ¢Ç‚Ç∑Ç¢
VECTOR  Math::NormRotMatrix(VECTOR norm, float rotation, int type) {
	MATRIX conv_matrix = MGetIdent();
	VECTOR re_vec = VGet(0, 0, 0);
	switch (type) {
	case 0://x
		conv_matrix = MGetRotX(rotation);
		break;
	case 1://y
		conv_matrix = MGetRotY(rotation);
		break;
	case 2://z
		conv_matrix = MGetRotZ(rotation);
		break;
	default:
		return VGet(-1,0,0);
	}
	re_vec = VTransform(norm, conv_matrix);

	return VNorm(re_vec);
}

//ï˚å¸ÉxÉNÉgÉãÇ™ó~ÇµÇ¢éûÇ…égÇ¢Ç‹Ç∑ÅB
VECTOR Math::MatrixToVector(MATRIX matrix,int index) {
	return VGet(matrix.m[index][0], matrix.m[index][1], matrix.m[index][2]);
}

// ï™ó£é≤Ç…ìäâeÇ≥ÇÍÇΩé≤ê¨ï™Ç©ÇÁìäâeê¸ï™í∑ÇéZèo
float  Math::LenSegOnSeparateAxis(VECTOR Sep, VECTOR e1, VECTOR e2, VECTOR e3 ) {

	// 3Ç¬ÇÃì‡êœÇÃê‚ëŒílÇÃòaÇ≈ìäâeê¸ï™í∑ÇåvéZ
	// ï™ó£é≤SepÇÕïWèÄâªÇ≥ÇÍÇƒÇ¢ÇÈÇ±Ç∆
	float r1 = fabs(VDot(Sep, e1));
	float r2 = fabs(VDot(Sep, e2));
	float r3 = 0.0f;
	if (e3.x != 0 || e3.y != 0 || e3.z != 0) {
		r3 = fabs(VDot(Sep, e3));
	}
	return r1 + r2 + r3;
}

MATRIX Math::MMultXYZ(float dir_x, float dir_y, float dir_z) {
	MATRIX matrix;
	matrix = MGetRotX(dir_x);
	matrix = MMult(matrix, MGetRotY(dir_y));
	matrix = MMult(matrix, MGetRotZ(dir_z));

	return matrix;
};

MATRIX Math::MMultXYZToMatrix(float dir_x, float dir_y, float dir_z) {
	MATRIX matrix;
	matrix = MGetRotX(dir_x);
	matrix = MMult(matrix, MGetRotY(dir_y));
	matrix = MMult(matrix, MGetRotZ(dir_z));
	matrix = MMult(matrix, GetCameraBillboardMatrix());

	return matrix;
};

bool Math::SetMatrixToVector(VECTOR& vec_x, VECTOR& vec_y, VECTOR& vec_z, const MATRIX matrix) {
	vec_x = VGet(matrix.m[0][0], matrix.m[0][1], matrix.m[0][2]);
	vec_y = VGet(matrix.m[1][0], matrix.m[1][1], matrix.m[1][2]);
	vec_z = VGet(matrix.m[2][0], matrix.m[2][1], matrix.m[2][2]);
	return true;
}

//âsäpÇ©Ç«Ç§Ç©
bool Math::CheckAcuteAngle(VECTOR p1, VECTOR p2, VECTOR p3) {
	return VDot(VSub(p1, p2), VSub(p3, p2)) >= 0.0f;
}

//ïΩçsÇ©Ç«Ç§Ç©
bool Math::CheckParallelRelation(VECTOR line_1_start, VECTOR line_1_end, VECTOR line_2_start, VECTOR line_2_end) {
	VECTOR line_1 = VSub(line_1_end, line_1_start);
	VECTOR line_2 = VSub(line_2_end, line_2_start);
	float value = VSize(VCross(line_1, line_2));
	float effective_range = 0.001f;
	return -effective_range <= value && value <= effective_range;
}

//êÇíºÇ©Ç«Ç§Ç©
bool Math::CheckVerticalRelation(VECTOR line_1_start, VECTOR line_1_end, VECTOR line_2_start, VECTOR line_2_end) {
	VECTOR line_1 = VSub(line_1_end, line_1_start);
	VECTOR line_2 = VSub(line_2_end, line_2_start);
	float dot = VDot(line_1, line_2);
	float effective_range = 0.001f;
	return -effective_range <= dot && dot <= effective_range;
}

float Math::CalcVectorAngle(VECTOR v1, VECTOR v2)
{
	float v1Size = VSize(v1);
	float v2Size = VSize(v2);
	if (v1Size > 0.000000f && v2Size > 0.000000f) {
		float cos = VDot(v1, v2) / VSize(v1) * VSize(v2);
		return acos(cos);
	}
	return 0;
}
