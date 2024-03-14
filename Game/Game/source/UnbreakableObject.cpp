#include "UnbreakableObject.h"

UnbreakableObject::UnbreakableObject()
{
	_modelHandle = -1;
	_pos = VGet(0.0f, 0.0f, 0.0f);
}

UnbreakableObject::~UnbreakableObject()
{
}

void UnbreakableObject::Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength)
{
	BuildingBase::Init(modelHandle, startPos, rotation, scale, obbLength);
	_canBreak = false;
}

void UnbreakableObject::Process()
{
}

void UnbreakableObject::DrawDebugInfo()
{
	BuildingBase::DrawDebugInfo();
}
