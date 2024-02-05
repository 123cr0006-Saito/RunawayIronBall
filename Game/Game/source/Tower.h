#pragma once
#include "BuildingBase.h"

class Tower : public BuildingBase
{
public:
	Tower();
	~Tower();

private:
	
	struct TOWER_PARTS_INFO {
		// この要素が使用中かどうか
		bool use;
		// パーツのモデルハンドル
		int modelHandle;
		// パーツの位置
		VECTOR pos;
		// 各軸における1フレームの回転値
		VECTOR vRot;
	};

	// パーツの情報
	std::vector<TOWER_PARTS_INFO> _partsInfo;
};