//----------------------------------------------------------------------
// @filename BoardArrow.cpp
// ＠date: 2024/02/01
// ＠author: saito ko
// @explanation
// 指定の方向に矢印を描画するクラス
//----------------------------------------------------------------------
#include "BoardArrow.h"
const unsigned short ::BoardArrow::vertexList[6] = { 0,1,2,2,1,3 };
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param name: ファイル名
// @param length: 矢印の長さ
//----------------------------------------------------------------------
BoardArrow::BoardArrow(std::string name,float length):
	_handle(ResourceServer::LoadGraph("Arrow", name.c_str()))
{
	// 初期化
	// uvリスト
	float uvList[4][2] = {
		{0.0 ,0.0 },
		{1.0f,0.0 },
		{0.0 ,1.0f},
		{1.0f,1.0f}
	};
	
	// 横の長さ
	float Width = 50.0f;
	// 縦の長さ
	_length = length;
	float posListX[4] = { -Width,Width, -Width ,Width };
	float posListZ[4] = { _length,_length,0.0f,0.0f };
	// 頂点の設定
	for (int i = 0; i < 4; i++) {
		_originPos[i] = VGet(posListX[i], 50, posListZ[i]);
		vertex[i].norm = VGet(0,1,0);
		vertex[i].dif = GetColorU8(255, 255, 255, 255);
		vertex[i].spc = GetColorU8(0, 0, 0, 0);
		vertex[i].u = uvList[i][0];
		vertex[i].v = uvList[i][1];
		vertex[i].su = 0.0f;
		vertex[i].sv = 0.0f;
	}
};

BoardArrow::~BoardArrow() {

};

bool BoardArrow::Update(VECTOR pos, float dirY) {
	// y軸行列を使用し回転させる
	MATRIX matrix = MGetRotY(dirY);
	for (int i = 0; i < 4; i++) {
		vertex[i].pos = VAdd(pos,VTransform(_originPos[i], matrix));
	}
	return true;
};

bool BoardArrow::Update(VECTOR pos, VECTOR dirVec) {
	// 方向ベクトルから角度を出し、y軸行列で回転させる
	float dirY = atan2(dirVec.x, dirVec.z);
	MATRIX matrix = MGetRotY(dirY);
	for (int i = 0; i < 4; i++) {
		vertex[i].pos = VAdd(pos, VTransform(_originPos[i], matrix));
	}
	return true;
};

bool BoardArrow::Render() {
	DrawPolygonIndexed3D(vertex, VERTEX_MAX, vertexList,  2, _handle, true);
	return true;
};