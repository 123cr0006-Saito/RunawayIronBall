#pragma once
#include "DxLib.h"		// VECTORやMATRIXを定義として使うため、.hに入れておく
#include "Easing.h"
#include "../../Header/Collision/MyStructure.h"

// 画像の中心で回転して描画する
void MyDrawModiGraph(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int cgHandle);

// 画像の中心で回転して描画する(左右反転版)
void MyDrawTurnModiGraph(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int cgHandle);

// 4点を指定して四角を描く
void MyDraw4PointBox(MATRIX mView, VECTOR pos[4], unsigned int Color, int FillFlag);

// 矩形を描く
void MyDrawBox(MATRIX mView, int x1, int y1, int x2, int y2, unsigned int Color, int FillFlag);

void MyDrawExtendGraph(MATRIX mView, int x1, int y1, int x2, int y2, unsigned int handle, int FillFlag);

void MyDrawDeformationGraph(MATRIX mView, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, float direction, int handle, int FillFlag);

void MyDrawRect(MATRIX mView, MYRECT_PLUS rc, unsigned int Color, int FillFlag);

void MyDrawLine(MATRIX mView, int x1, int y1, int x2, int y2, int Color);

void MyDrawCircle(MATRIX mView, int x, int y, int r);

// 位置を揃えて文字列を描く
// posBaseX = -1:左揃え, 0:中央, 1:右揃え
void DrawPosString(int x, int y, int posBaseX, unsigned int color, const char* str);

//キャラだけを塗りつぶす
void PortalIn(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int GrHandle, int Color, int turn, float param);