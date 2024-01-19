#pragma once
#include "appframe.h"
#include "BuildingBase.h"


class House : public BuildingBase
{
	typedef BuildingBase base;

public:
	House();
	~House();

	bool Init(int modelHandle, VECTOR startPos) override;
	bool Process() override;
	bool Render() override;


protected:
	OBB obb;

	// �z�u���W���烂�f���̒��S���W�܂ł̍����x�N�g��
	VECTOR vDiffToCenter;
};