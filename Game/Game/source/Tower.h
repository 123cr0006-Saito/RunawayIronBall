//----------------------------------------------------------------------
// @filename Tower.cpp
// ＠date: 2024/04/01
// ＠author: Morozumi Hiroya
// @explanation
// タワーオブジェクトの制御・描画を行うクラス
// 1つのタワーオブジェクトは、複数のTowerPartsクラスから構成される
//----------------------------------------------------------------------
#pragma once
#include "TowerParts.h"
#include "ObjectBase.h"

class Tower : public ObjectBase
{
public:
	Tower();
	virtual ~Tower();


	void Init(std::vector<int> modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale);
	void Process();
	void Render();

	bool GetUse() { return _use; }

	VECTOR GetPos() { return _pos; }

	void SetBlast(VECTOR vDir);
	bool GetCanBlast() { return _canBlast; }


	Sphere GetCollision() { return _sphereCollision; }


	// デバッグ情報の表示
	void DrawDebugInfo();

protected:
	// タワーパーツが残っているかどうか
	bool _use;
	// 起点座標
	VECTOR _pos;

	// パーツの情報
	std::vector<TowerParts*> _towerParts;

	// パーツの初期の数
	int _partsNum;
	
	bool _isFalling;
	int _prevFallCnt;
	int _fallCnt;

	bool _canBlast;

	// 最下部のパーツのインデックス
	int _bottomIndex;


	Sphere _sphereCollision;

};