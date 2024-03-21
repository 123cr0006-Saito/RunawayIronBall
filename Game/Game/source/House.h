#pragma once
#include "appframe.h"
#include "BuildingBase.h"
#include "BreakObject.h"


class House : public BuildingBase
{
public:
	House();
	~House();

	void Init(int modelHandle,std::string objName, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength, int hp, int exp);
	void Process() override;

	void SetHit(VECTOR vDir = VGet(0.0f, 0.0f, -1.0f)) override;


	// デバッグ情報の表示
	void DrawDebugInfo() override;

protected:
	// 破壊処理の有効化
	void ActivateBreakObject(bool activate, VECTOR vDir = VGet(0.0f, 0.0f, 0.0f));

	int _hp;
	int _exp;
	// 破壊処理クラス
	BreakObject* _breakObj;
};