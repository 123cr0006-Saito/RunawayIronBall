#pragma once
#include "appframe.h"
#include "ObjectBase.h"
#include "BreakObject.h"

class BuildingBase : public ObjectBase
{
public:
	BuildingBase();
	virtual ~BuildingBase();

	virtual void Init(int modelHandle, std::string objName,VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength);
	virtual void Process();
	virtual void Render();

	VECTOR GetPos() { return _pos; }

	bool GetUseCollision() { return _useCollision; }
	void SetUseCollision(bool b);
	bool GetCanBreak() { return _canBreak; }
	OBB GetOBBCollision() { return _obbCollision; }

	std::string GetName() {return _objName;}
	int GetSuppression() { return _suppression; }


	virtual void SetHit(VECTOR vDir = VGet(0.0f, 0.0f, -1.0f)) {};

	// デバッグ情報の表示
	virtual void DrawDebugInfo();

protected:
	// モデルハンドル
	int _modelHandle;
	std::string _objName;
	int _hp; // HP
	int _exp; // 経験値
	int _suppression; // 制圧値
	// モデルの座標
	VECTOR _pos;

	// 当たり判定を行うかどうか
	bool _useCollision;
	// 壊せるオブジェクトかどうか
	bool _canBreak;
	// 当たり判定
	OBB _obbCollision;
};