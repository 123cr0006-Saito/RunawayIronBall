//----------------------------------------------------------------------
// @filename BreakableBuilding.cpp
// ＠date: 2024/04/01
// ＠author: Morozumi Hiroya
// @explanation
// 破壊可能な建物のクラス
//----------------------------------------------------------------------
#include "BreakableBuilding.h"

BreakableBuilding::BreakableBuilding()
{
	_breakModelParts = nullptr;
}

BreakableBuilding::~BreakableBuilding()
{
	SAFE_DELETE(_breakModelParts);
}

// 初期化処理
void BreakableBuilding::Init(int modelHandle, std::string objName, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength, int hp, int exp, int suppression)
{
	// 基底クラスの初期化処理
	BuildingBase::Init(modelHandle, objName,startPos, rotation, scale, obbLength);

	// HP、経験値、制圧値の設定
	_hp = hp;
	_exp = exp;
	_suppression = suppression;

	// 破壊可能状態にする
	_canBreak = true;

	// 破壊処理クラスの初期化
	_breakModelParts = NEW BreakModelParts();
	_breakModelParts->Init(_modelHandle);
}

// 更新処理
void BreakableBuilding::Process()
{
	// 破壊処理
	// 有効状態かどうかの判定はBreakModelPartsクラス内で行う
	_breakModelParts->Process();
}

// 被ダメージ設定
void BreakableBuilding::SetHit(VECTOR vDir)
{
	// 破壊処理の開始
	ActivateBreakObject(true, vDir);
}

// 破壊処理の有効化
void BreakableBuilding::ActivateBreakObject(bool activate, VECTOR vDir)
{
	_breakModelParts->Activate(activate, vDir);
	SetUseCollision(false);
}

// デバッグ情報の表示
void BreakableBuilding::DrawDebugInfo()
{
	BuildingBase::DrawDebugInfo();
	_breakModelParts->DrawDebugInfo();
}
