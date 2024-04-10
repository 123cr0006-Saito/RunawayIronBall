#include "UnbreakableBuilding.h"

UnbreakableBuilding::UnbreakableBuilding()
{
	_modelHandle = -1;
	_pos = VGet(0.0f, 0.0f, 0.0f);
}

UnbreakableBuilding::~UnbreakableBuilding()
{
}

void UnbreakableBuilding::Init(int modelHandle, std::string objName, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength)
{
	BuildingBase::Init(modelHandle,  objName, startPos, rotation, scale, obbLength);
	_canBreak = false;
	 _hp = 0;
	 _exp = 0; 
	 _suppression = 0; 
}

void UnbreakableBuilding::Process()
{
}

void UnbreakableBuilding::DrawDebugInfo()
{
	BuildingBase::DrawDebugInfo();
}
