#pragma once
#include "appframe.h"

#include "EnemyPool.h"

class Cell
{
public:
	Cell() {
		_segment = nullptr;
		_obj = nullptr;
		_shouldUpdateCollision = false;
		_prev = nullptr;
		_next = nullptr;
	}
	~Cell() {
		_segment = nullptr;
		_obj = nullptr;
		_prev = nullptr;
		_next = nullptr;
	}

	Cell* _segment;
	EnemyBase* _obj;
	bool _shouldUpdateCollision;

	Cell* _prev;
	Cell* _next;
};

// XZ���ʏ��4���؋�ԕ������s���A�����蔻����s���N���X
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void Init();
	void UpdateTree();

	void AddObject(EnemyBase* enemy);
	unsigned int CalcTreeIndex(VECTOR pos1, VECTOR pos2);
	void InsertNewObjectIntoTree(unsigned int treeIndex, EnemyBase* enemy);
	void InsertCellIntoTree(unsigned int treeIndex, Cell* cell);

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