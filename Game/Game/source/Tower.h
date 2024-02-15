#pragma once
#include "BuildingBase.h"

class Tower : public BuildingBase
{
	typedef BuildingBase base;

public:
	Tower();
	~Tower();

	//bool Init(int modelHandle, VECTOR startPos) override;
	bool Init(std::array<int, 3> modelHandle, VECTOR startPos);
	bool Process() override;
	bool Render() override;

	// デバッグ情報の表示
	bool DrawDebugInfo() override;

protected:
	
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
	std::vector<TOWER_PARTS_INFO*> _partsInfo;
};