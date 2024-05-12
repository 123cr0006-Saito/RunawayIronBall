//----------------------------------------------------------------------
// @filename Cell.h
// @date: 2024/03/07
// @author: Morozumi Hiroya
// 
// @explanation
// �����蔻�菈���̊Ǘ����s��CollisionManager�N���X�ɏ���o�^���邽�߂̃N���X
// �iCollisionManager�N���X�FXZ���ʂɑ΂���4���؋�ԕ������s���A�����蔻�菈���̉񐔂��팸���邽�߂̃N���X�j
// CollisionManager�N���X�ŁA���������Z�O�����g���Ƃɑ�����Cell�̑o�������X�g���\�z����
//----------------------------------------------------------------------
#pragma once
#include "ObjectBase.h"

// ����Cell�C���X�^���X��ێ����Ă���I�u�W�F�N�g�̎��
enum OBJ_TYPE {
	NONE,
	PL,
	PL_IB,
	PL_IB_CHAIN,
	EN,
	EN_IB,
	BLDG,
	TWR,
	TWR_PRT,
};

class ObjectBase;
class Cell
{
public:
	Cell() {
		_segment = nullptr;
		_obj = nullptr;
		_objType = OBJ_TYPE::NONE;
		_prev = nullptr;
		_next = nullptr;
	}
	~Cell() {
		_segment = nullptr;
		_obj = nullptr;
		_prev = nullptr;
		_next = nullptr;
	}

	// �����Ă���Z�O�����g�iCollisionManager�N���X���ō\�z����o�������X�g�̐擪�̃_�~�[�Z���j
	Cell* _segment;
	// ����Cell�C���X�^���X��ێ����Ă���I�u�W�F�N�g
	ObjectBase* _obj;
	// ����Cell�C���X�^���X��ێ����Ă���I�u�W�F�N�g�̎��
	OBJ_TYPE _objType;

	// �o�������X�g�̑O��̃Z��
	Cell* _prev;
	Cell* _next;
};