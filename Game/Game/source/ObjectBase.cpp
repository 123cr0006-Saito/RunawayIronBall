#include "ObjectBase.h"
#include "Application/UtilMacro.h"

ObjectBase::ObjectBase()
{
	_cell = new Cell();
	_cell->_obj = this;
}

ObjectBase::~ObjectBase()
{
	SAFE_DELETE(_cell);
}
