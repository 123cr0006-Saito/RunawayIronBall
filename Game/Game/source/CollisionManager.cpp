#include "CollisionManager.h"
#include <math.h>

namespace {
	constexpr float STAGE_LENGTH = 27000.0f;
	constexpr int STAGE_DIVISION = 1;
}

CollisionManager::CollisionManager()
{
	_offsetX = 0.0f;
	_offsetZ = 0.0f;
	_segmentNumPerSide = 0;
	_segmentLength = 0.0f;
	treeSize = 0;
	_tree.clear();

	B* b = new B();
	Test1* t1 = new Test2();
	Test1* t = b->GetTest2();

	Test2* t2 = dynamic_cast<Test2*>(t);
	Test3* t3 = dynamic_cast<Test3*>(t);

	EnemyBase* e = new SlaBlock();
	SlaBlock* s = dynamic_cast<SlaBlock*>(e);
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Init()
{
	_offsetX = -STAGE_LENGTH / 2.0f;
	_offsetZ = -STAGE_LENGTH / 2.0f;
	_segmentNumPerSide = pow(2, STAGE_DIVISION);
	_segmentLength = STAGE_LENGTH / static_cast<float>(_segmentNumPerSide);


	treeSize = (pow(4, STAGE_DIVISION + 1) - 1) / 3;
	_tree.resize(treeSize);

	for(int i = 0; i < treeSize; i++) {
		_tree[i] = new Cell();
	}
}

void CollisionManager::AddEnemy(EnemyBase* enemy)
{
	if(enemy == nullptr) return;
	unsigned int areaIndex = CheckArea(enemy->GetCollisionPos());
	Cell* prevCell = _tree[areaIndex];
	Cell* newCell = new Cell();
	newCell->_obj = enemy;
	newCell->_segment = prevCell;
	while (prevCell->_next != nullptr)
	{
		prevCell = prevCell->_next;
	}
	prevCell->_next = newCell;
	newCell->_prev = prevCell;
}

unsigned int CollisionManager::CheckArea(VECTOR pos)
{
	VECTOR p = VGet(pos.x - _offsetX, 0.0f, pos.z - _offsetZ);
	p.x = Math::Clamp(0.0f, STAGE_LENGTH, p.x);
	p.z = Math::Clamp(0.0f, STAGE_LENGTH, p.z);
	unsigned int xIndex = static_cast<unsigned int>(p.x / _segmentLength);
	unsigned int zIndex = static_cast<unsigned int>(p.z / _segmentLength);


	unsigned int areaIndex = (SeparateBit(xIndex) | (SeparateBit(zIndex) << 1));

	return areaIndex;
}

unsigned int CollisionManager::SeparateBit(unsigned int n)
{
	n = (n | (n << 8)) & 0x00ff00ff;
	n = (n | (n << 4)) & 0x0f0f0f0f;
	n = (n | (n << 2)) & 0x33333333;
	return (n | (n << 1)) & 0x55555555;
}

void CollisionManager::CheckHit()
{
	for(int i = 0; i < treeSize; i++) {
		Cell* cell1 = _tree[i]->_next;
		while (cell1 != nullptr)
		{

			EnemyBase* en1 = cell1->_obj;
			VECTOR en1Pos = en1->GetCollisionPos();
			float en1R = en1->GetR();
			Cell* cell2 = cell1->_next;

			while (cell2 != nullptr)
			{
				EnemyBase* en2 = cell2->_obj;
				VECTOR en2Pos = en2->GetCollisionPos();
				float en2R = en2->GetR();
				VECTOR vDir = VSub(en2Pos, en1Pos);
				float sqLength = VSquareSize(vDir);
				if(sqLength < (en1R + en2R) * (en1R + en2R)) {
					float length = sqrt(sqLength);
					vDir = VScale(vDir, 1.0f / length);
					en2->SetExtrusionPos(VScale(vDir, (en1R + en2R) - length));
				}
				cell2 = cell2->_next;
			}
			cell1 = cell1->_next;
		}
	}
}

void CollisionManager::DrawSegmentLine()
{
	for(int i = 0; i < _segmentNumPerSide + 1; i++)	{
		VECTOR startPos = VGet(_offsetX + _segmentLength * i, 0.0f, _offsetZ);
		VECTOR endPos = VAdd(startPos, VGet(0.0f, 0.0f, STAGE_LENGTH));

		DrawLine3D(startPos, endPos, COLOR_BLUE);
	}
	for (int i = 0; i < _segmentNumPerSide + 1; i++) {
		VECTOR startPos = VGet(_offsetX, 0.0f, _offsetZ + _segmentLength * i);
		VECTOR endPos = VAdd(startPos, VGet(STAGE_LENGTH, 0.0f, 0.0f));

		DrawLine3D(startPos, endPos, COLOR_RED);
	}
}

void CollisionManager::DrawAreaIndex()
{
	for(int i = 0; i < treeSize; i++) {
		Cell* cell = _tree[i];
		while (cell->_next != nullptr)
		{
			EnemyBase* e = cell->_obj;
			if(e == nullptr) {
				cell = cell->_next;
				continue;
			}
			VECTOR screenPos = ConvWorldPosToScreenPos(e->GetCollisionPos());
			DrawFormatString(screenPos.x, screenPos.y, COLOR_WHITE, "%d", i);
			cell = cell->_next;
		}
	}
}


