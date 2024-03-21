#pragma once
#include "BuildingBase.h"

class UnbreakableObject : public BuildingBase
{
public:
	UnbreakableObject();
	virtual ~UnbreakableObject();

	void Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength);
	void Process() override;

	// �f�o�b�O���̕\��
	void DrawDebugInfo() override;

protected:

};