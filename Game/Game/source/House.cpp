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

	vDiffToCenter = VGet(0.0f, -250.0f, 0.0f);
	obb.pos = VSub(_pos, vDiffToCenter);
	obb.length[0] = 500.0f;
	obb.length[1] = 500.0f;
	obb.length[2] = 500.0f;

	return true;
}

bool House::Process()
{
	if(!base::Process()) { return false; }
	return true;
}

bool House::Render()
{
	if (!base::Render()) { return false; }	
	return true;
}

bool House::DrawDebugInfo()
{
	if (!base::DrawDebugInfo()) { return false; }
	obb.Render(GetColor(255, 255, 255));
	return true;
}
