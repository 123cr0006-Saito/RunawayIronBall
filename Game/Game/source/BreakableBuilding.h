//----------------------------------------------------------------------
// @filename BreakableBuilding.h
// @date: 2024/04/01
// @author: Morozumi Hiroya
// @explanation
// 破壊可能な建物のクラス
//----------------------------------------------------------------------
#pragma once
#include "BuildingBase.h"
#include "BreakModelParts.h"

class BreakableBuilding : public BuildingBase
{
public:
	BreakableBuilding();
	virtual ~BreakableBuilding();

	// 初期化処理
	void Init(int modelHandle,std::string objName, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength, int hp, int exp,int suppression);
	// 更新処理
	void Process() override;

	// 被ダメージ設定
	void SetHit(VECTOR vDir = VGet(0.0f, 0.0f, -1.0f)) override;

	// デバッグ情報の表示
	void DrawDebugInfo() override;

protected:
	// 破壊処理の有効化
	void ActivateBreakObject(bool activate, VECTOR vDir = VGet(0.0f, 0.0f, 0.0f));

	// 破壊処理クラス
	BreakModelParts* _breakModelParts;
};