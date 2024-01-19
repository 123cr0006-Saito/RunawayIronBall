#include "House.h"

House::House() :BuildingBase::BuildingBase() {
	vDiffToCenter = VGet(0.0f, 0.0f, 0.0f);
}

House::~House()
{
}

bool House::Init(int modelHandle, VECTOR startPos)
{
	if (!base::Init(modelHandle, startPos)){ return false;}

	obb.pos = _pos;
	
	return true;
}

bool House::Process()
{
	return true;
}

bool House::Render()
{
	return true;
}
