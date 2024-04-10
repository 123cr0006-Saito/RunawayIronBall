#pragma once
#include "BuildingBase.h"

class UnbreakableBuilding : public BuildingBase
{
public:
	UnbreakableBuilding();
	 virtual ~UnbreakableBuilding();

	void Init(int modelHandle, std::string objName, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength);
	void Process() override;

	// デバッグ情報の表示
	void DrawDebugInfo() override;

protected:

};