#include "CharacterBase.h"

CharacterBase::CharacterBase(int modelHandle, VECTOR pos)
{
	this->_modelHandle = modelHandle;
	this->_pos = pos;
	_forwardDir = VGet(0.0f, 0.0f, -1.0f);
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
