//----------------------------------------------------------------------
// @filename BoardParabola.cpp
// @date: 2024/02/01
// @author: saito ko
// @explanation
// 指定の方向に放物線の矢印を描画するクラス
//----------------------------------------------------------------------
#include "BoardParabola.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param name : ファイル名
// @return なし
//----------------------------------------------------------------------
BoardParabola::BoardParabola(std::string name) :
	_handle(ResourceServer::LoadGraph("Arrow",name.c_str()))
{
	// 頂点情報の初期化
	for (int i = 0; i < VERTEX_MAX * (ONE_ROW_POLYGON_MAX / 2) + 2; i++) {
		_vertex[i].dif = GetColorU8(200, 255, 255, 100);
		_vertex[i].norm = VGet(0.0f, 1.0f, 0.0f);
		_vertex[i].spc = GetColorU8(0, 0, 0, 0);
	}

	// 頂点情報の設定
	float uvPos = 0;
	for (int i = 0; i <= ONE_ROW_POLYGON_MAX; i++) {
		_vertex[i * 2].u = uvPos / ONE_ROW_POLYGON_MAX;
		_vertex[i * 2].v = 0;
		_vertex[i * 2 + 1].u = uvPos / ONE_ROW_POLYGON_MAX;
		_vertex[i * 2 + 1].v = 1.0f;
		uvPos++;
	}
	// 頂点インデックスの設定
	for (int i = 0; i < ONE_ROW_POLYGON_MAX; i++) {
		_vertexOrder[i * 6] = 0 + 2 * i;
		_vertexOrder[i * 6 + 1] = 2 + 2 * i;
		_vertexOrder[i * 6 + 2] = 1 + 2 * i;
		_vertexOrder[i * 6 + 3] = 1 + 2 * i;
		_vertexOrder[i * 6 + 4] = 2 + 2 * i;
		_vertexOrder[i * 6 + 5] = 3 + 2 * i;
	}
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return なし
//----------------------------------------------------------------------
BoardParabola::~BoardParabola() {
};
//----------------------------------------------------------------------
// @brief 更新処理
// @param pos : 位置
// @param dirY : Y軸の回転
// @param length : +z軸の長さ
// @param inclination : 傾き
// @return 成功したかどうか
//----------------------------------------------------------------------
bool BoardParabola::Update(VECTOR pos, float dirY, float length, float inclination) {
	// 必要な変数の宣言
	MATRIX matrix = MGetRotY(dirY);
	float harfLength = length / 2;
	float Length = -harfLength;
	// 頂点情報の設定
	for (int i = 0; i <= ONE_ROW_POLYGON_MAX; i++) {
		// 頂点の座標を設定
		float depth = (Length + harfLength) * -1;
		_vertex[i * 2].pos.x = 50;
		_vertex[i * 2].pos.y = -inclination * pow(Length, 2) + inclination * pow(harfLength, 2);
		_vertex[i * 2].pos.z = depth;
		// 頂点の座標を行列で変換
		_vertex[i * 2].pos = VTransform(_vertex[i * 2].pos, matrix);
		_vertex[i * 2].pos = VAdd(pos, _vertex[i * 2].pos);
		// 頂点の座標を設定
		_vertex[i * 2 + 1].pos.x = -50;
		_vertex[i * 2 + 1].pos.y = -inclination * pow(Length, 2) + inclination * pow(harfLength, 2);
		_vertex[i * 2 + 1].pos.z = depth;
		// 頂点の座標を行列で変換
		_vertex[i * 2 + 1].pos = VTransform(_vertex[i * 2 + 1].pos, matrix);
		_vertex[i * 2 + 1].pos = VAdd(pos, _vertex[i * 2 + 1].pos);
		// 長さを増やす
		Length += length / ONE_ROW_POLYGON_MAX;
	}
	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool BoardParabola::Render() {
	DrawPolygonIndexed3D(_vertex, VERTEX_MAX / 2 * ONE_ROW_POLYGON_MAX + 2, _vertexOrder, ONE_ROW_POLYGON_MAX * 2, _handle, true);
	return true;
};