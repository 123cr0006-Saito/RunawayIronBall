#include "../../Header/Collision/2DCollision.h"
#include "dxlib.h"



// �����蔻��p�B2��box�������������𔻒�
// �������Ă�����true, �������Ă��Ȃ�������false��Ԃ�
bool TwoDimCol::IsHitBox(
	float x1, float y1, float w1, float h1,		// �ЂƂ߂�box ����(x,y), �傫��w,h
	float x2, float y2, float w2, float h2		// �ӂ��߂�box ����(x,y), �傫��w,h
) {
	if (x1 < x2 + w2 && x2 < x1 + w1		// x�����̔���
		&& y1 < y2 + h2 && y2 < y1 + h1		// y�����̔���
		)
	{
		// 2��box�͓������Ă���
		return true;
	}

	// 2��box�͓������Ă��Ȃ�
	return false;
}

bool TwoDimCol::IsHitBox(MYRECT rect1, MYRECT rect2) {
	return IsHitBox(rect1.x, rect1.y, rect1.w, rect1.h, rect2.x, rect2.y, rect2.w, rect2.h);
};

// �����蔻��p�B2�̉~�������������𔻒�
// �������Ă�����true, �������Ă��Ȃ�������false��Ԃ�
bool TwoDimCol::IsHitCircle(
	float x1, float y1, float r1,		// �ЂƂ߂�circle ���S(x,y), ���ar
	float x2, float y2, float r2		// �ӂ��߂�circle ���S(x,y), ���ar
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

// �����蔻��p�B�~�Ƌ�`�������������𔻒�
// �������Ă�����true, �������Ă��Ȃ�������false��Ԃ�
bool TwoDimCol::IsHitCircleRect(
	float cx1, float cy1, float r,          // �~�̔���
	float x2, float y2, float w, float h    // ��`�̔���
) {
	float x, y;
	float rad, deg;
	// 360�_�Ƃ��ē����蔻�肵����ꍇ�ɂ���ďd���Ȃ�܂���
	for (deg = 0; deg <= 360; deg += 90) {
		rad = Math::DegToRad(deg);
		x = (cx1 + r * cos(rad));
		y = (cy1 + r * sin(rad));

		// ����N�_�̋�`�̂݁i���Ԃ��Ȃ��̂Łj
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

		if (circle_x < box_x) { //��
			if (circle_y < box_y) {  //����
				if (PythagoreanLeft > PythagoreanRight) {
					hitfrag = false;
				}
			}

			if (box_y + box_h < circle_y) {  //����
				if (PythagoreanLeft > PythagoreanRight) {
					hitfrag = false;
				}
			}
		}

		if (circle_x > box_x) { //�E
			if (box_y > circle_y) {  //�E��
				if (PythagoreanLeft > PythagoreanRight) {
					hitfrag = false;
				}

			}
			if (box_y + box_h < circle_y) {  //�E��
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