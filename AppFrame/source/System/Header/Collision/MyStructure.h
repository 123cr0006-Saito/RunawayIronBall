#pragma once
#include "dxlib.h"

// 円周率
#define	PI				(3.141592653589793)

// RECT用構造体
struct MYRECT {
	float		x, y, w, h;
};

// RECT用構造体+アルファ
struct MYRECT_PLUS : public MYRECT{
	float cx, cy;
	float direction;
};

struct MYCIRCLE {
	float x, y;
	float r;
};

struct HITSLIDE {
	int	hit;				// 当たっていれば1、当たっていないければ0
	int	slide_x, slide_y;	// 当たっている場合、ずらすべき値
	int index;
};

struct POINT_LINE_SHORT {
	VECTOR hit_point = VGet(0, 0, 0);
	float length = 0.0f;
	float coefficient = 0.0f;//ベクトル係数
};

struct TWOLINE_SHORT {
	VECTOR line_1_point = VGet(0, 0, 0);
	VECTOR line_2_point = VGet(0, 0, 0);
	float line_1_coefficient = 0.0f;
	float line_2_coefficient = 0.0f;
	float length = 0.0f;
};

struct OBB {
	VECTOR pos = VGet(0, 0, 0);
	VECTOR Misalignment = VGet(0, 0, 0);//モデルのずれ修正用
	VECTOR dir_vec[3] = { VGet(0,0,0),VGet(0,0,0),VGet(0,0,0) };//xv,yv,zv
	float direction[3] = { 0,0,0 };//0:x 1:y 2:z
	float length[3] = {0.0f,0.0f,0.0f}; //0:w 1:h 3:d
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

	VECTOR up_pos = VGet(0, 0, 0);
	VECTOR down_pos = VGet(0, 0, 0);
	float up = 0.0f;
	float r = 0.0f;
	float direction[3] = { 0.0f,0.0f, 0.0f };
};