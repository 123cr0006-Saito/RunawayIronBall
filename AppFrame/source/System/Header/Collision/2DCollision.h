#pragma once
#include <math.h>
#include "../../Header/Collision/MyStructure.h"
#include "../Function/mymath.h"
#include "dxlib.h"

class TwoDimCol
{
public:

	// �����蔻��p�B2��box�������������𔻒�
	// �������Ă�����1, �������Ă��Ȃ�������0��Ԃ�
	static bool IsHitBox(
		float x1, float y1, float w1, float h1,		// �ЂƂ߂�box ����(x,y), �傫��w,h
		float x2, float y2, float w2, float h2		// �ӂ��߂�box ����(x,y), �傫��w,h
	);

	// �����蔻��p�B2�̉~�������������𔻒�
	// �������Ă�����1, �������Ă��Ȃ�������0��Ԃ�
	static bool IsHitCircle(
		float x1, float y1, float r1,		// �ЂƂ߂�circle ���S(x,y), ���ar
		float x2, float y2, float r2		// �ӂ��߂�circle ���S(x,y), ���ar
	);

	static bool IsHitCircleRect(
		float cx1, float cy1, float r,               // �~�̔���
		float x2, float y2, float w, float h         // ��`�̔���      
	);

	static bool IsHitCircleAndBox(float box_x, float  box_y, float  box_w, float  box_h, float circle_x, float circle_y, float circle_r);   //�~�Ǝl�p�̓����蔻��

	static bool IsHitMultBox(float bullet_x, float bullet_y, float bullet_w, float bullet_h, float bullet_cx, float bullet_cy, float bullet_direction,
								          float player_x, float player_y, float player_w, float player_h); //������`�Ƃ̓����蔻��
};