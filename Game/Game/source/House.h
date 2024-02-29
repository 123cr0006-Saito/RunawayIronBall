#pragma once
#include "appframe.h"
#include "BuildingBase.h"


class House : public BuildingBase
{
	typedef BuildingBase base;

public:
	House();
	~House();

	bool Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength);
	bool Process() override;
	bool Render() override;

	OBB GetOBBCollision() { return obb; }

	// �f�o�b�O���̕\��
	bool DrawDebugInfo() override;

protected:
	// �����蔻��p
	OBB obb;

	static std::vector<int> _breakFrame;
};