//----------------------------------------------------------------------
// @filename UnbreakableBuilding.h
// ＠date: 2024/04/01
// ＠author: Morozumi Hiroya
// @explanation
// 破壊不可能な建物のクラス
//----------------------------------------------------------------------
#pragma once
#include "BuildingBase.h"

class UnbreakableBuilding : public BuildingBase
{
public:
	UnbreakableBuilding();
	 virtual ~UnbreakableBuilding();

	// 初期化処理
	void Init(int modelHandle, std::string objName, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength);
	// 更新処理
	void Process() override;

	// デバッグ情報の表示
	void DrawDebugInfo() override;

protected:

};