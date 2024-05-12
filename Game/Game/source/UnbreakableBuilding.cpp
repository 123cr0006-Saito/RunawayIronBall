//----------------------------------------------------------------------
// @filename UnbreakableBuilding.cpp
// @date: 2024/03/05
// @author: Morozumi Hiroya
// @explanation
// 破壊不可能な建物のクラス
//----------------------------------------------------------------------
#include "UnbreakableBuilding.h"

UnbreakableBuilding::UnbreakableBuilding()
{
	_modelHandle = -1;
	_pos = VGet(0.0f, 0.0f, 0.0f);
}

UnbreakableBuilding::~UnbreakableBuilding()
{
}

// 初期化処理
void UnbreakableBuilding::Init(int modelHandle, std::string objName, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength)
{
	// 基底クラスの初期化処理
	BuildingBase::Init(modelHandle, objName, startPos, rotation, scale, obbLength);
	// 破壊不可能状態にする
	_canBreak = false;
}

// 更新処理
void UnbreakableBuilding::Process()
{
}

// デバッグ情報の表示
void UnbreakableBuilding::DrawDebugInfo()
{
	BuildingBase::DrawDebugInfo();
}
