#include "../../Header/Function/mymath.h"
 //degree��radian
float Math::DegToRad(float deg) {
	return deg * PI / 180.0f;		// �p�x��degree����radian�ɕϊ�
}

// radian��degree
float Math::RadToDeg(float rad) {
	return rad * 180.0f / PI;		// �p�x��radian����degree�ɕϊ�
}

// �N�����v
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

//�}�g���N�X�����x�N�g���𐳋K�����ēn��
//���[�v���Ŏg���₷��
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

//�����x�N�g�����~�������Ɏg���܂��B
VECTOR Math::MatrixToVector(MATRIX matrix,int index) {
	return VGet(matrix.m[index][0], matrix.m[index][1], matrix.m[index][2]);
}

// �������ɓ��e���ꂽ���������瓊�e���������Z�o
float  Math::LenSegOnSeparateAxis(VECTOR Sep, VECTOR e1, VECTOR e2, VECTOR e3 ) {

	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	// ������Sep�͕W��������Ă��邱��
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

// �s�p���ǂ���
bool Math::CheckAcuteAngle(VECTOR p1, VECTOR p2, VECTOR p3) {
	return VDot(VSub(p1, p2), VSub(p3, p2)) >= 0.0f;
}

// ���s���ǂ���
bool Math::CheckParallelRelation(VECTOR line_1_start, VECTOR line_1_end, VECTOR line_2_start, VECTOR line_2_end) {
	VECTOR line_1 = VSub(line_1_end, line_1_start);
	VECTOR line_2 = VSub(line_2_end, line_2_start);
	float value = VSize(VCross(line_1, line_2));
	float effective_range = 0.001f;
	return -effective_range <= value && value <= effective_range;
}

// �������ǂ���
bool Math::CheckVerticalRelation(VECTOR line_1_start, VECTOR line_1_end, VECTOR line_2_start, VECTOR line_2_end) {
	VECTOR line_1 = VSub(line_1_end, line_1_start);
	VECTOR line_2 = VSub(line_2_end, line_2_start);
	float dot = VDot(line_1, line_2);
	float effective_range = 0.001f;
	return -effective_range <= dot && dot <= effective_range;
}

// 2�̃x�N�g���̂Ȃ��p���v�Z����
// �߂�l: �p�x(radian) 
float Math::CalcVectorAngle(VECTOR v1, VECTOR v2)
{
	float v1SquareSize = VSquareSize(v1);
	float v2SquareSize = VSquareSize(v2);
	if (v1SquareSize > 0.000000f && v2SquareSize > 0.000000f) {
		float cos = VDot(v1, v2) / (sqrt(v1SquareSize) * sqrt(v2SquareSize));
		return acos(cos);
	}
	return 0;
}

// 3D���f��������vForward�̕��������ʂɂȂ�悤�ɉ�]������
void Math::SetModelForward_RotationY(int modelHandle, VECTOR vForwad)
{
	VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
	vForwad.y = 0.0f;
	float angle = CalcVectorAngle(vBase, vForwad);
	angle *= vForwad.x < 0.0f ? 1.0f : -1.0f;
	MV1SetRotationXYZ(modelHandle, VGet(0, angle, 0));
}
