#include "CharacterBase.h"

CharacterBase::CharacterBase()
{
	_modelHandle = -1;
	_pos = VGet(0.0f, 0.0f, 0.0f);
	_forwardDir = VGet(0.0f, 0.0f, -1.0f);
}

CharacterBase::~CharacterBase()
{
	if (_modelHandle != -1) {
		MV1DeleteModel(_modelHandle);
	}
}

bool CharacterBase::Init(int modelHandle, VECTOR pos)
{
	this->_modelHandle = modelHandle;
	this->_pos = pos;
	return true;
}

bool CharacterBase::Process()
{

	return true;
}

bool CharacterBase::Render()
{
	MV1SetPosition(_modelHandle, _pos);
	MV1DrawModel(_modelHandle);
	return true;
}
