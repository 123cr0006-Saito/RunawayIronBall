//----------------------------------------------------------------------
// @filename ObjectBase.cpp
// @date: 2024/03/07
// @author: Morozumi Hiroya
// 
// @explanation
// �����蔻�菈�����s���S�ẴI�u�W�F�N�g�̊��N���X
// �����蔻�菈���̊Ǘ��Ɏg�p����CoollisionManager�N���X��Cell�N���X�̃C���X�^���X��ێ�����
// �iCollisionManager�N���X�FXZ���ʂɑ΂���4���؋�ԕ������s���A�����蔻�菈���̉񐔂��팸���邽�߂̃N���X�j
// �iCell�N���X�FCollisionManager�N���X�ɏ���o�^���邽�߂̃N���X�j
//----------------------------------------------------------------------
#include "ObjectBase.h"
#include "Application/UtilMacro.h"

ObjectBase::ObjectBase()
{
	_collisionManager = CollisionManager::GetInstance();
	_cell = NEW Cell();
	_cell->_obj = this;
}

ObjectBase::~ObjectBase()
{
	_collisionManager = nullptr;
	SAFE_DELETE(_cell);
}
