#include "ObjectBase.h"
#include "Application/UtilMacro.h"

ObjectBase::ObjectBase()
{
	_cell = new Cell();
	_cell->_obj = this;
	_collisionManager = CollisionManager::GetInstance();
}

ObjectBase::~ObjectBase()
{
	SAFE_DELETE(_cell);
}

void ObjectBase::LoadModel() {
	_modelHandle = ResourceServer::MV1LoadModel(_name.c_str(), _path.c_str());
};
