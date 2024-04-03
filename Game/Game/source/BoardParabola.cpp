//----------------------------------------------------------------------
// @filename BoardParabola.cpp
// ＠date: 2024/02/01
// ＠author: saito ko
// @explanation
// 指定の方向に放物線の矢印を描画するクラス
//----------------------------------------------------------------------
#include "BoardParabola.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param name : ファイル名
//----------------------------------------------------------------------
BoardParabola::BoardParabola(std::string name) :
	_handle(ResourceServer::LoadGraph("Arrow",name.c_str()))
{
	for (int i = 0; i < VERTEX_MAX * (ONE_ROW_POLYGON_MAX / 2) + 2; i++) {
		vertex[i].dif = GetColorU8(200, 255, 255, 100);

		vertex[i].norm = VGet(0.0f, 1.0f, 0.0f);
		vertex[i].spc = GetColorU8(0, 0, 0, 0);
	}

	float uvPos = 0;
	for (int i = 0; i <= ONE_ROW_POLYGON_MAX; i++) {
		vertex[i * 2].u = uvPos / ONE_ROW_POLYGON_MAX;
		vertex[i * 2].v = 0;
		vertex[i * 2 + 1].u = uvPos / ONE_ROW_POLYGON_MAX;
		vertex[i * 2 + 1].v = 1.0f;
		uvPos++;
	}

	for (int i = 0; i < ONE_ROW_POLYGON_MAX; i++) {
		vertexOrder[i * 6] = 0 + 2 * i;
		vertexOrder[i * 6 + 1] = 2 + 2 * i;
		vertexOrder[i * 6 + 2] = 1 + 2 * i;
		vertexOrder[i * 6 + 3] = 1 + 2 * i;
		vertexOrder[i * 6 + 4] = 2 + 2 * i;
		vertexOrder[i * 6 + 5] = 3 + 2 * i;
	}
};

BoardParabola::~BoardParabola() {
};

bool BoardParabola::Update(VECTOR pos, float dirY, float length, float inclination) {
	MATRIX matrix = MGetRotY(dirY);
	float harfLength = length / 2;
	float Length = -harfLength;

	for (int i = 0; i <= ONE_ROW_POLYGON_MAX; i++) {
		float okuyuki = (Length + harfLength) * -1;
		vertex[i * 2].pos.x = 50;
		vertex[i * 2].pos.y = -inclination * pow(Length, 2) + inclination * pow(harfLength, 2);
		vertex[i * 2].pos.z = okuyuki;
		vertex[i * 2].pos = VTransform(vertex[i * 2].pos, matrix);
		vertex[i * 2].pos = VAdd(pos, vertex[i * 2].pos);
		vertex[i * 2 + 1].pos.x = -50;
		vertex[i * 2 + 1].pos.y = -inclination * pow(Length, 2) + inclination * pow(harfLength, 2);
		vertex[i * 2 + 1].pos.z = okuyuki;
		vertex[i * 2 + 1].pos = VTransform(vertex[i * 2 + 1].pos, matrix);
		vertex[i * 2 + 1].pos = VAdd(pos, vertex[i * 2 + 1].pos);
		Length += length / ONE_ROW_POLYGON_MAX;
	}
	return true;
};

bool BoardParabola::Render() {
	DrawPolygonIndexed3D(vertex, VERTEX_MAX / 2 * ONE_ROW_POLYGON_MAX + 2, vertexOrder, ONE_ROW_POLYGON_MAX * 2, _handle, true);
	return true;
};