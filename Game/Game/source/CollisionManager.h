#pragma once
#include "appframe.h"
#include "Cell.h"
#include "EnemyBase.h"

// XZ���ʏ��4���؋�ԕ������s���A�����蔻����s���N���X
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void Init();
	void UpdateTree();


	void UpdateCell(Cell* cell);

	unsigned int CalcTreeIndex(VECTOR pos1, VECTOR pos2);
	void InsertCellIntoTree(unsigned int treeIndex, Cell* cell);
	void RemoveCellFromTree(Cell* cell);

	unsigned int CheckArea(VECTOR pos);

	unsigned int SeparateBit(unsigned int n);


	void CheckHit();

	// �f�o�b�O���̕\��
	void DrawSegmentLine();

	void DrawAreaIndex();

private:
	float _offsetX;
	float _offsetZ;

	int _segmentNumPerSide;
	float _segmentLength;


	int treeSize;
	std::vector<Cell*> _tree;
};