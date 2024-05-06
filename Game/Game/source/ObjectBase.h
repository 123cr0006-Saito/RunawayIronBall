//----------------------------------------------------------------------
// @filename ObjectBase.h
// @date: 2024/03/07
// @author: Morozumi Hiroya
// 
// @explanation
// �����蔻�菈�����s���S�ẴI�u�W�F�N�g�̊��N���X
// �����蔻�菈���̊Ǘ��Ɏg�p����CoollisionManager�N���X��Cell�N���X�̃C���X�^���X��ێ�����
// �iCollisionManager�N���X�FXZ���ʂɑ΂���4���؋�ԕ������s���A�����蔻�菈���̉񐔂��팸���邽�߂̃N���X�j
// �iCell�N���X�FCollisionManager�N���X�ɏ���o�^���邽�߂̃N���X�j
//----------------------------------------------------------------------
#pragma once
#include "Cell.h"
#include "CollisionManager.h"

class CollisionManager;
class Cell;

class ObjectBase
{
public:
	ObjectBase();
	virtual ~ObjectBase();

	// CollisionManager�N���X�̃C���X�^���X
	CollisionManager* _collisionManager;
	// Cell�N���X�̃C���X�^���X
	Cell* _cell;
};