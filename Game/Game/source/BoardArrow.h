//----------------------------------------------------------------------
// @filename BoardArrow.h
// ＠date: 2024/02/01
// ＠author: saito ko
// @explanation
// 指定の方向に矢印を描画するクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include <string>
class BoardArrow
{
public:
	BoardArrow(std::string name, float length);
	~BoardArrow();
	bool Update(VECTOR pos, float dirY); //ｙ軸角度
	bool Update(VECTOR pos, VECTOR dirVec); // 方向ベクトル
	bool Render();
protected:
	int _handle;// 画像ハンドル
	float _length;// 矢印の長さ
	static const int VERTEX_MAX = 4;// 頂点数
	static const unsigned short vertexList[6];// 頂点リスト
	VERTEX3D vertex[VERTEX_MAX];// 頂点情報
	VECTOR _originPos[4];// 矢印の原点
};