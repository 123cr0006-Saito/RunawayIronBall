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

	// デバッグ情報の表示
	bool DrawDebugInfo() override;

protected:
	OBB obb;

	// 配置座標からモデルの中心座標までの差分ベクトル
	VECTOR vDiffToCenter;
};