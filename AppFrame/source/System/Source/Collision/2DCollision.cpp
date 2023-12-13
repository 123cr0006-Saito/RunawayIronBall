#include "../../Header/Collision/2DCollision.h"
#include "dxlib.h"



// 当たり判定用。2つのboxが当たったかを判定
// 当たっていたらtrue, 当たっていなかったらfalseを返す
bool TwoDimCol::IsHitBox(
	float x1, float y1, float w1, float h1,		// ひとつめのbox 左上(x,y), 大きさw,h
	float x2, float y2, float w2, float h2		// ふたつめのbox 左上(x,y), 大きさw,h
) {
	if (x1 < x2 + w2 && x2 < x1 + w1		// x方向の判定
		&& y1 < y2 + h2 && y2 < y1 + h1		// y方向の判定
		)
	{
		// 2つのboxは当たっている
		return true;
	}

	// 2つのboxは当たっていない
	return false;
}

bool TwoDimCol::IsHitBox(MYRECT rect1, MYRECT rect2) {
	return IsHitBox(rect1.x, rect1.y, rect1.w, rect1.h, rect2.x, rect2.y, rect2.w, rect2.h);
};

// 当たり判定用。2つの円が当たったかを判定
// 当たっていたらtrue, 当たっていなかったらfalseを返す
bool TwoDimCol::IsHitCircle(
	float x1, float y1, float r1,		// ひとつめのcircle 中心(x,y), 半径r
	float x2, float y2, float r2		// ふたつめのcircle 中心(x,y), 半径r
) {
	float w, h, r;
	w = x1 - x2;
	h = y1 - y2;
	r = r1 + r2;
	if (r * r > w * w + h * h) {
		return true;
	}
	return false;
}

// 当たり判定用。円と矩形が当たったかを判定
// 当たっていたらtrue, 当たっていなかったらfalseを返す
bool TwoDimCol::IsHitCircleRect(
	float cx1, float cy1, float r,          // 円の判定
	float x2, float y2, float w, float h    // 矩形の判定
) {
	float x, y;
	float rad, deg;
	// 360点とって当たり判定したら場合によって重くなりました
	for (deg = 0; deg <= 360; deg += 90) {
		rad = Math::DegToRad(deg);
		x = (cx1 + r * cos(rad));
		y = (cy1 + r * sin(rad));

		// 左上起点の矩形のみ（時間がないので）
		if (x2 < x && x < x2 + w && y2 < y && y < y2 + h) {
			return true;
		}
	}

	return false;
}

bool TwoDimCol::IsHitCircleAndBox(float box_x, float  box_y, float  box_w, float  box_h, float circle_x, float circle_y, float circle_r) {
	int hitfrag = false;

	if ((circle_x > box_x - circle_r)
		&& (circle_y > box_y - circle_r)
		&& (circle_x < box_x + box_w + circle_r)
		&& (circle_y < box_y + box_h + circle_r)) {

		hitfrag = true;
		float PythagoreanRight = circle_r * circle_r;
		int PythagoreanLeft = ((box_x - circle_x) * (box_x - circle_x)) + ((box_y - circle_y) * (box_y - circle_y));

		if (circle_x < box_x) { //左
			if (circle_y < box_y) {  //左上
				if (PythagoreanLeft > PythagoreanRight) {
					hitfrag = false;
				}
			}

			if (box_y + box_h < circle_y) {  //左上
				if (PythagoreanLeft > PythagoreanRight) {
					hitfrag = false;
				}
			}
		}

		if (circle_x > box_x) { //右
			if (box_y > circle_y) {  //右上
				if (PythagoreanLeft > PythagoreanRight) {
					hitfrag = false;
				}

			}
			if (box_y + box_h < circle_y) {  //右上
				if (PythagoreanLeft > PythagoreanRight) {
					hitfrag = false;
				}

			}

		}
	}
	return hitfrag;
}

bool TwoDimCol::IsHitMultBox(float bullet_x, float bullet_y, float bullet_w, float bullet_h, float bullet_cx, float bullet_cy, float bullet_direction,
	              float player_x, float player_y, float player_w, float player_h) {

	struct BULLET_BOX {
		double x, y;
		double w, h;
		float direction;
	};

	BULLET_BOX bullet_box[3];

	bullet_box[0] = { bullet_x - bullet_cx - bullet_w * cos(bullet_direction), bullet_y - bullet_cy - bullet_h * sin(bullet_direction), bullet_w , bullet_h,bullet_direction };
	bullet_box[1] = { bullet_x - bullet_cx, bullet_y - bullet_cy, bullet_w, bullet_h,bullet_direction };
	bullet_box[2] = { bullet_x - bullet_cx + bullet_w * cos(bullet_direction), bullet_y - bullet_cy + bullet_h * sin(bullet_direction) , bullet_w , bullet_h ,bullet_direction };

	for (int i = 0; i < 3; i++) {
		if (IsHitBox(bullet_box[i].x, bullet_box[i].y, bullet_box[i].w, bullet_box[i].h, player_x, player_y, player_w, player_h) == 1) {
			return true;
		}
	}

	return false;
}