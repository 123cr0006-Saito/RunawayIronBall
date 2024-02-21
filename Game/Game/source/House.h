#pragma once
#include "appframe.h"
#include "BuildingBase.h"


class House : public BuildingBase
{
	typedef BuildingBase base;

public:
	House();
	~House();

	bool Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale) override;
	bool Process() override;
	bool Render() override;

	OBB GetOBBCollision() { return obb; }

	// デバッグ情報の表示
	bool DrawDebugInfo() override;

protected:
	// 当たり判定用
	OBB obb;

	// 配置座標からモデルの中心座標までの差分ベクトル
	// obbの位置を求めるために使用
	VECTOR vDiffToCenter;

	static std::vector<int> _breakFrame;
};