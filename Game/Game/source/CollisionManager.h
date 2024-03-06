#pragma once
#include "appframe.h"

#include "EnemyPool.h"

class Cell
{
public:
	Cell() {
		_segment = nullptr;
		_obj = nullptr;
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

	Cell* _prev;
	Cell* _next;
};

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void Init();

	void AddEnemy(EnemyBase* enemy);

	unsigned int CheckArea(VECTOR pos);

	unsigned int SeparateBit(unsigned int n);


	void CheckHit();

	// デバッグ情報の表示
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