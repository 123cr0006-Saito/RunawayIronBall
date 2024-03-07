#pragma once
#include "appframe.h"
#include "BreakObject.h"


class House
{
public:
	House();
	~House();

	void Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength);
	void Process();
	void Render();

	VECTOR GetPos() { return _pos; }

	bool GetUseCollision() { return _useCollision; }
	void SetUseCollision(bool b) { _useCollision = b; }
	OBB GetOBBCollision() { return obb; }

	void ActivateBreakObject(bool activate, VECTOR vDir = VGet(0.0f, 0.0f, 0.0f));




	// デバッグ情報の表示
	void DrawDebugInfo();

protected:
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