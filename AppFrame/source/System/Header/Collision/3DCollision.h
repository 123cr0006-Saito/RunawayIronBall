#pragma once
#include "MyStructure.h"
#include "../../Header/Collision/2DCollision.h"
#include"dxlib.h"
class Collision3D
{
public:

	//�ŒZ�����͑�̓_�Ɠ_�𐂒��ɉ��������ɂȂ�

	//���ʏ�ł̎l�p���m�̂����蔻��
	static bool PlaneCollision(VECTOR pos1, float w1, float d1, VECTOR pos2, float w2, float d2);

	//AABB�̓����蔻��
	static bool AABBCollision(VECTOR pos1, float w1, float h1, float d1, VECTOR pos2, float w2, float h2, float d2);

	//���ʏ�ł̉~�̂����蔻��
	static bool CircleCollision(VECTOR pos1, float r1, VECTOR pos2, float r2);

	//�~���̓����蔻��
	static bool CylindCollision(VECTOR pos1, float r1,float h1, VECTOR pos2, float r2,float h2);

	//AABB�̍ŒZ����
	static float AABBShortLength(VECTOR Box, float wide, float height, float depth, VECTOR Point);

	//OBB���m�̓����蔻��
	static bool OBBCollision(OBB box_1,OBB box_2,bool flag = false);

    //�_�ƒ����̍ŒZ����
	static POINT_LINE_SHORT PointLineShortLength(VECTOR line_start, VECTOR line_end, VECTOR point);

	//�_�Ɛ����̍ŒZ����   ���I�ȍ��͎n�_�ƏI�_�̓��[���h���W�œ����
	static POINT_LINE_SHORT PointLineSegShortLength(VECTOR line_start, VECTOR line_end, VECTOR point);

	//2�����̐����ɂȂ�ꏊ�ƃx�N�g���W��
	static TWOLINE_SHORT TwoLineShortPoint(VECTOR line_1_start, VECTOR line_1_end, VECTOR line_2_start, VECTOR line_2_end);

	//2�����̐����ɂȂ�ꏊ�ƃx�N�g���W��
	static TWOLINE_SHORT TwoSegmentShortPoint(VECTOR line_1_start, VECTOR line_1_end, VECTOR line_2_start, VECTOR line_2_end);

	//���Ƌ��̓����蔻��
	static bool SphereCol(VECTOR pos1, float r1, VECTOR pos2, float r2);

	//�J�v�Z�����m�̓����蔻��
	static bool TwoCapselCol(VECTOR line_1_start, VECTOR line_1_end, float r_1, VECTOR line_2_start, VECTOR line_2_end, float r_2);

	//�_�Ƃn�a�a�̍Őڋߓ_�@OBB�Ɠ_����ԋ߂�OBB�̈ʒu��Ԃ��܂��B
	static  VECTOR PointOBB(VECTOR point, OBB obb);

	static VECTOR PointOBBToBillBoard(VECTOR point, OBB obb);
	
	//OBB�Ƌ��̓����蔻��
	static bool OBBSphereCol(OBB obb, VECTOR point, float r);

	//OBB�ƃJ�v�Z���̓����蔻��
	static bool OBBCapselCol(VECTOR line_start, VECTOR line_end, OBB obb, float r);
	//�I�[�o�[���[�h
	static bool OBBCapselCol(Capsule capsule, OBB obb);
	static bool OBBToBillBoardCapselCol(Capsule capsule, OBB obb);

};