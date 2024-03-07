#pragma once
#include "appframe.h"
#include "BreakObject.h"

class BuildingBase
{
public:
	BuildingBase();
	virtual ~BuildingBase();

	virtual bool Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale);
	virtual bool Process();
	virtual bool Render();

	void ActivateBreakObject(bool activate, VECTOR vDir = VGet(0.0f, 0.0f, 0.0f));

	VECTOR GetPos() { return _pos; }

	bool GetUseCollision() { return _useCollision; }
	void SetUseCollision(bool b) { _useCollision = b; }

	// デバッグ情報の表示
	virtual bool DrawDebugInfo();

protected:
	// 当たり判定を行うかどうか
	bool _useCollision;

	int _modelHandle;
	VECTOR _pos;

	BreakObject* _breakObj;
};