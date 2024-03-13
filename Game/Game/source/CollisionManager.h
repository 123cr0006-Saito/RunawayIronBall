#pragma once
#include "appframe.h"
#include "Cell.h"
#include "ObjectBase.h"
class CharacterBase;
class Player;
class EnemyBase;
class BuildingBase;
class IronBall;

// XZ平面上で4分木空間分割を行い、当たり判定を行うクラス
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	static CollisionManager* GetInstance() { return _instance; }

	void Init();
	void Process();

	void UpdateCell(Cell* cell);
	void RemoveCellFromTree(Cell* cell);


	// デバッグ情報の表示
	void DrawSegmentLine();
	void DrawAreaIndex();

private:
	unsigned int CalcTreeIndex(VECTOR pos1, VECTOR pos2);
	unsigned int CheckArea(VECTOR pos);
	unsigned int SeparateBit(unsigned int n);
	void InsertCellIntoTree(unsigned int treeIndex, Cell* cell);
	void CreateColList(unsigned int treeIndex, std::list<Cell*>& colStack);
	void CheckColList();

	// 当たり判定処理
	void CheckHit(Player* player, EnemyBase* enemy);
	void CheckHit(Player* player, BuildingBase* building);

	void CheckHit(EnemyBase* enemy1, EnemyBase* enemy2);
	void CheckHit(EnemyBase* enemy, BuildingBase* building);

	static CollisionManager* _instance;

	float _offsetX;
	float _offsetZ;

	int _segmentNumPerSide;
	float _segmentLength;


	int _treeSize;
	std::vector<Cell*> _tree;

	// 当たり判定を行うセルのリスト
	std::list<std::pair<Cell*, Cell*>> _colList;
};