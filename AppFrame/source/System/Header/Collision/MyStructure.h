#pragma once
#include "dxlib.h"
#include <vector>

// �~����
#define	PI				(3.141592653589793)

// RECT�p�\����
struct MYRECT {
	float		x, y, w, h;
};

// RECT�p�\����+�A���t�@
struct MYRECT_PLUS : public MYRECT{
	float cx, cy;
	float direction;
};

struct MYCIRCLE {
	float x, y;
	float r;
};

struct HITSLIDE {
	int	hit;				// �������Ă����1�A�������Ă��Ȃ������0
	int	slide_x, slide_y;	// �������Ă���ꍇ�A���炷�ׂ��l
	int index;
};

struct POINT_LINE_SHORT {
	VECTOR hit_point = VGet(0, 0, 0);
	float length = 0.0f;
	float coefficient = 0.0f;//�x�N�g���W��
};

struct TWOLINE_SHORT {
	VECTOR line_1_point = VGet(0, 0, 0);
	VECTOR line_2_point = VGet(0, 0, 0);
	float line_1_coefficient = 0.0f;
	float line_2_coefficient = 0.0f;
	float length = 0.0f;
};

//struct OBB {
//	VECTOR pos = VGet(0, 0, 0);
//	VECTOR dir_vec[3] = { VGet(0,0,0),VGet(0,0,0),VGet(0,0,0) };//xv,yv,zv
//	float length[3] = {0.0f,0.0f,0.0f}; //0:w 1:h 3:d
//};

class Sphere
{
public:
	Sphere() {
		centerPos = VGet(0.0f, 0.0f, 0.0f);
		r = 0.0f;
	}
	Sphere(VECTOR pos, float r) {
		centerPos = pos;
		this->r = r;
	}

	// �`�揈��
	void Render(unsigned int color);

	VECTOR centerPos;
	float r;
};

class OBB
{
public:
	OBB() {
		pos = VGet(0.0f, 0.0f, 0.0f);

		// ������Ԃł̓��[���h�̎��ƕ��s�ȏ�Ԃɂ���iAABB�j
		dir_vec[0] = VGet(1.0f, 0.0f, 0.0f);
		dir_vec[1] = VGet(0.0f, 1.0f, 0.0f);
		dir_vec[2] = VGet(0.0f, 0.0f, 1.0f);

		for (int i = 0; i < 3; ++i) {			
			length[i] = 0.0f;
		}
	}

	// ��]����
	// x��->y��->z���̏��Ԃ�, �e������]������
	void Rotate(VECTOR vRot);

	// ���_���W�̎擾
	void GetVertexPos(std::vector<VECTOR>& vertexPosList);

	// �`�揈��
	void Render(unsigned int color);

	VECTOR pos;
	VECTOR dir_vec[3];//xv,yv,zv
	float length[3]; //0:w 1:h 3:d
};

class Capsule {
public:
	Capsule() {
		up_pos = VGet(0, 0, 0);
		down_pos = VGet(0, 0, 0);
		up = 0.0f;
		r = 0.0f;
		for (int i = 0; i < 3; i++) {
			direction[i] = 0;
		}
		
	};

	void Update() {
		up_pos = VAdd(down_pos,VGet(0,up,0));
	};

	// �`�揈��
	void Render(unsigned int color);

	VECTOR up_pos = VGet(0, 0, 0);
	VECTOR down_pos = VGet(0, 0, 0);
	float up = 0.0f;
	float r = 0.0f;
	float direction[3] = { 0.0f,0.0f, 0.0f };
};