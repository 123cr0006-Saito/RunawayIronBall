#pragma once
#include "appframe.h"
#include "ObjectBase.h"
#include "BreakObject.h"

class BuildingBase : public ObjectBase
{
public:
	BuildingBase();
	virtual ~BuildingBase();

	virtual void Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength);
	virtual void Process();
	virtual void Render();

	VECTOR GetPos() { return _pos; }

	bool GetUseCollision() { return _useCollision; }
	void SetUseCollision(bool b) { _useCollision = b; }
	bool GetCanBreak() { return _canBreak; }
	OBB GetOBBCollision() { return obb; }


	void SetHit(VECTOR vDir = VGet(0.0f, 0.0f, -1.0f)) {};

	// デバッグ情報の表示
	virtual void DrawDebugInfo();

protected:
	// モデルハンドル
	int _modelHandle;
	// モデルの座標
	VECTOR _pos;

	// 当たり判定を行うかどうか
	bool _useCollision;
	// 壊せるオブジェクトかどうか
	bool _canBreak;
	// 当たり判定
	OBB obb;
};