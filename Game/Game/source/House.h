#pragma once
#include "appframe.h"
#include "BuildingBase.h"
#include "BreakObject.h"


class House : public BuildingBase
{
public:
	House();
	~House();

	void Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength) override;
	void Process() override;
	void Render() override;

	void SetHit(VECTOR vDir = VGet(0.0f, 0.0f, -1.0f)) override;


	// デバッグ情報の表示
	void DrawDebugInfo();

protected:
	// 破壊処理の有効化
	void ActivateBreakObject(bool activate, VECTOR vDir = VGet(0.0f, 0.0f, 0.0f));

	// モデルハンドル
	int _modelHandle;
	// モデルの座標
	VECTOR _pos;

	// 当たり判定を行うかどうか
	bool _useCollision;
	// 当たり判定
	OBB obb;

	// 破壊処理クラス
	BreakObject* _breakObj;
};