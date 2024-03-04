#pragma once
#include "TowerParts.h"

class Tower
{
public:
	Tower();
	~Tower();


	void Init(std::array<int, 3> modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale);
	void Process();
	void Render();

	bool GetUse() { return _use; }

	VECTOR GetPos() { return _pos; }

	void SetBlast(VECTOR vDir);
	bool GetCanBlast() { return _canBlast; }

	void UpdateCollision();


	Sphere GetBottomSphereCollision() { return _towerParts[_bottomIndex]->GetSphereCollision(); }


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


	Sphere* _bottomSphereCollision;

};