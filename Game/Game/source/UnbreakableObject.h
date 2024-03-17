#pragma once
#include "BuildingBase.h"

class UnbreakableObject : public BuildingBase
{
public:
	UnbreakableObject();
	virtual ~UnbreakableObject();

	void Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength);
	void Process() override;

	// デバッグ情報の表示
	void DrawDebugInfo() override;

protected:

};