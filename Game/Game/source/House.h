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

	// デバッグ情報の表示
	bool DrawDebugInfo() override;

protected:
	// 当たり判定用
	OBB obb;

	static std::vector<int> _breakFrame;
};