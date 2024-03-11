#pragma once
#include "dxlib.h"
#include <vector>

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

	// 描画処理
	void Render(unsigned int color);

	VECTOR centerPos;
	float r;
};

class OBB
{
public:
	OBB() {
		pos = VGet(0.0f, 0.0f, 0.0f);

		// 初期状態ではワールドの軸と平行な状態にする（AABB）
		dir_vec[0] = VGet(1.0f, 0.0f, 0.0f);
		dir_vec[1] = VGet(0.0f, 1.0f, 0.0f);
		dir_vec[2] = VGet(0.0f, 0.0f, 1.0f);

		for (int i = 0; i < 3; ++i) {			
			length[i] = 0.0f;
		}
	}

	// 回転処理
	// x軸->y軸->z軸の順番で, 各軸を回転させる
	void Rotate(VECTOR vRot);

	// 頂点座標の取得
	void GetVertexPos(std::vector<VECTOR>& vertexPosList);

	// 描画処理
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

	// 描画処理
	void Render(unsigned int color);

	VECTOR up_pos = VGet(0, 0, 0);
	VECTOR down_pos = VGet(0, 0, 0);
	float up = 0.0f;
	float r = 0.0f;
	float direction[3] = { 0.0f,0.0f, 0.0f };
};