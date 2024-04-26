//----------------------------------------------------------------------
// @filename BoardParabola.h
// @date: 2024/02/01
// @author: saito ko
// @explanation
// 指定の方向に放物線の矢印を描画するクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include <string>
class BoardParabola
{
public:
	BoardParabola(std::string name);
	~BoardParabola();
	bool Update(VECTOR pos, float dirY, float length, float inclination); // 放物線
	bool Render();
protected:
	int _handle;// 画像ハンドル
	static const int ONE_ROW_POLYGON_MAX = 100;//ポリゴンの最大数
	static const int VERTEX_MAX = 4;//頂点の最大数
	unsigned short _vertexOrder[ONE_ROW_POLYGON_MAX * 6];// 頂点の順番
	VERTEX3D _vertex[ONE_ROW_POLYGON_MAX / 2 * VERTEX_MAX + 2];// 頂点情報
};

