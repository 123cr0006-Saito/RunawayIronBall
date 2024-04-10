//----------------------------------------------------------------------
// @filename BuildingBase.h
// ＠date: 2024/04/01
// ＠author: Morozumi Hiroya
// @explanation
// 建物クラスの基底クラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "ObjectBase.h"
#include "BreakObject.h"

class BuildingBase : public ObjectBase
{
public:
	BuildingBase();
	virtual ~BuildingBase();

	// 初期化処理
	virtual void Init(int modelHandle, std::string objName,VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength);
	// 更新処理
	virtual void Process();
	// 描画処理
	virtual void Render();

	std::string GetName() { return _objName; }
	virtual int GetHp() { return 0; }
	virtual int GetExp() { return 0; }
	int GetSuppression() { return _suppression; }

	VECTOR GetPos() { return _pos; }

	bool GetCanBreak() { return _canBreak; }

	// 当たり判定の使用設定
	void SetUseCollision(bool b);

	bool GetUseCollision() { return _useCollision; }
	OBB GetOBBCollision() { return _obbCollision; }

	// 被ダメージ設定
	// Houseクラスでオーバーライド
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

	// 壊せるオブジェクトかどうか
	// 継承先で設定
	bool _canBreak;

	// 当たり判定を行うかどうか
	bool _useCollision;
	// 当たり判定
	OBB _obbCollision;
};