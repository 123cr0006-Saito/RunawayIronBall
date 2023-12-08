#pragma once
#include <math.h>
#include "../../Header/Collision/MyStructure.h"
#include "../Function/mymath.h"
#include "dxlib.h"

class TwoDimCol
{
public:

	// 当たり判定用。2つのboxが当たったかを判定
	// 当たっていたら1, 当たっていなかったら0を返す
	static bool IsHitBox(
		float x1, float y1, float w1, float h1,		// ひとつめのbox 左上(x,y), 大きさw,h
		float x2, float y2, float w2, float h2		// ふたつめのbox 左上(x,y), 大きさw,h
	);

	static bool IsHitBox(
		MYRECT rect1,		// ひとつめのMYRECT 左上(x,y), 大きさw,h
		MYRECT rect2		// ふたつめのMYRECT 左上(x,y), 大きさw,h
	);

	// 当たり判定用。2つの円が当たったかを判定
	// 当たっていたら1, 当たっていなかったら0を返す
	static bool IsHitCircle(
		float x1, float y1, float r1,		// ひとつめのcircle 中心(x,y), 半径r
		float x2, float y2, float r2		// ふたつめのcircle 中心(x,y), 半径r
	);

	static bool IsHitCircleRect(
		float cx1, float cy1, float r,               // 円の判定
		float x2, float y2, float w, float h         // 矩形の判定      
	);

	static bool IsHitCircleAndBox(float box_x, float  box_y, float  box_w, float  box_h, float circle_x, float circle_y, float circle_r);   //円と四角の当たり判定

	static bool IsHitMultBox(float bullet_x, float bullet_y, float bullet_w, float bullet_h, float bullet_cx, float bullet_cy, float bullet_direction,
								          float player_x, float player_y, float player_w, float player_h); //複数矩形との当たり判定
};