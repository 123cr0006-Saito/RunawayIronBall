#pragma once
#include "appframe.h"
#include "Cell.h"
#include "EnemyBase.h"

// XZ平面上で4分木空間分割を行い、当たり判定を行うクラス
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	static CollisionManager* GetInstance(){ return _instance; }

	void Init();
	void Process();

	void UpdateCell(Cell* cell);

	unsigned int CalcTreeIndex(VECTOR pos1, VECTOR pos2);
	void RemoveCellFromTree(Cell* cell);

	unsigned int CheckArea(VECTOR pos);


	// デバッグ情報の表示
	void DrawSegmentLine();
	void DrawAreaIndex();

private:
	unsigned int SeparateBit(unsigned int n);
	void InsertCellIntoTree(unsigned int treeIndex, Cell* cell);
	void CreateColList(unsigned int treeIndex, std::list<Cell*>& colStack);
	void CheckColList();
	void CheckHit(EnemyBase* enemy1, EnemyBase* enemy2);

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