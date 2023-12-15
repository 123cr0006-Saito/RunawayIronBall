#include "CharacterBase.h"

CharacterBase::CharacterBase(int modelHandle, VECTOR pos)
{
	this->_modelHandle = modelHandle;
	this->_pos = pos;
	MV1SetPosition(this->_modelHandle, this->_pos);
}

CharacterBase::~CharacterBase()
{
	if (_modelHandle != -1) {
		MV1DeleteModel(_modelHandle);
	}
}

bool CharacterBase::Process()
{

	return true;
}

bool CharacterBase::Render()
{
	MV1DrawModel(_modelHandle);
	return true;
}
