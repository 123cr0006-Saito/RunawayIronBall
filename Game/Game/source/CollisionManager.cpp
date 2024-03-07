#include "CollisionManager.h"

#include "EnemyBase.h"


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

void CollisionManager::UpdateTree()
{
	//for (int i = 0; i < treeSize; i++) {
	//	Cell* cell = _tree[i]->_next;
	//	while (cell)
	//	{
	//		Cell* nextCell = cell->_next;

	//		EnemyBase* e = cell->_enObj;
	//		if (e == nullptr || e->GetUse() == false) {
	//			cell = cell->_next;
	//			continue;
	//		}
	//		VECTOR centerPos = e->GetCollisionPos();
	//		float r = e->GetR();
	//		VECTOR pos1 = VGet(centerPos.x - r, 0.0f, centerPos.z - r);
	//		VECTOR pos2 = VGet(centerPos.x + r, 0.0f, centerPos.z - r);


	//		unsigned int newIndex = CalcTreeIndex(pos1, pos2);
	//		if(newIndex != i) {
	//			Cell* prevCell = cell->_prev;
	//			prevCell->_next = nextCell;
	//			if(nextCell != nullptr) {
	//				nextCell->_prev = prevCell;
	//			}
	//			InsertCellIntoTree(newIndex, cell);
	//		}
	//		cell = nextCell;
	//	}
	//}
}

void CollisionManager::UpdateCell(Cell* cell)
{
	if(cell == nullptr || cell->_obj == nullptr) return;

	unsigned int treeIndex = 0;
	switch (cell->_objType)
	{
	case OBJ_TYPE::NONE:
		return;
		break;
	case OBJ_TYPE::PL:
		break;
	case OBJ_TYPE::PL_IB:
		break;
	case OBJ_TYPE::EN:
	{
		EnemyBase* enemy = static_cast<EnemyBase*>(cell->_obj);
		VECTOR centerPos = enemy->GetCollisionPos();
		float r = enemy->GetR();
		VECTOR pos1 = VGet(centerPos.x - r, 0.0f, centerPos.z - r);
		VECTOR pos2 = VGet(centerPos.x + r, 0.0f, centerPos.z - r);
		treeIndex = CalcTreeIndex(pos1, pos2);
	}
		break;
	case OBJ_TYPE::EN_IB:
		break;
	}

	if (cell->_segment != _tree[treeIndex]) {
		RemoveCellFromTree(cell);
		InsertCellIntoTree(treeIndex, cell);
	}
}

unsigned int CollisionManager::CalcTreeIndex(VECTOR pos1, VECTOR pos2)
{
	unsigned int areaIndex1 = CheckArea(pos1);
	unsigned int areaIndex2 = CheckArea(pos2);
	unsigned int xorIndex = areaIndex1 ^ areaIndex2;

	unsigned int areaIndex = areaIndex1;
	int shift = 0;
	if (xorIndex != 0) {
		int tmpShift = 0;

		for (int i = 0; i < STAGE_DIVISION; i++) {
			tmpShift += 2;
			// 下位2ビットを取り出す
			unsigned int bit = xorIndex & 0x11;
			if (bit != 0) {
				shift = tmpShift;
			}
			xorIndex = xorIndex >> 2;
		}
	}

	areaIndex = areaIndex >> shift;
	int parentDivNum = STAGE_DIVISION - shift / 2;
	int treeIndex = (pow(4, parentDivNum) - 1) / 3;
	treeIndex += areaIndex;

	return treeIndex;
}

void CollisionManager::InsertCellIntoTree(unsigned int treeIndex, Cell* cell)
{
	Cell* baseCell = _tree[treeIndex];
	Cell* nextCell = baseCell->_next;

	cell->_segment = baseCell;

	baseCell->_next = cell;
	cell->_prev = baseCell;

	if (nextCell != nullptr) {
		nextCell->_prev = cell;
		cell->_next = nextCell;
	}
}

void CollisionManager::RemoveCellFromTree(Cell* cell)
{
	Cell* prevCell = cell->_prev;
	Cell* nextCell = cell->_next;

	if (prevCell != nullptr) {
		prevCell->_next = nextCell;
	}
	if (nextCell != nullptr) {
		nextCell->_prev = prevCell;
	}
	cell->_segment = nullptr;
	cell->_prev = nullptr;
	cell->_next = nullptr;
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

			EnemyBase* en1 = dynamic_cast<EnemyBase*>(cell1->_obj);
			VECTOR en1Pos = en1->GetCollisionPos();
			float en1R = en1->GetR();
			Cell* cell2 = cell1->_next;

			while (cell2 != nullptr)
			{
				EnemyBase* en2 = dynamic_cast<EnemyBase*>(cell2->_obj);
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






	//int n = 0;
	//for (int i = 0; i < treeSize; i++) {
	//	Cell* cell = _tree[i]->_next;
	//	while (cell != nullptr)
	//	{
	//		n++;
	//		cell = cell->_next;
	//	}
	//}

	//int m = 0;
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
	for (int i = 0; i < treeSize; i++) {
		Cell* cell = _tree[i]->_next;
		while (cell != nullptr)
		{
			EnemyBase* e = dynamic_cast<EnemyBase*>(cell->_obj);
			if (e == nullptr || e->GetUse() == false) {
				cell = cell->_next;
				continue;
			}
			VECTOR screenPos = ConvWorldPosToScreenPos(e->GetCollisionPos());
			if (0.0f < screenPos.z && screenPos.z < 1.0f) {
				DrawFormatString(screenPos.x, screenPos.y, COLOR_RED, "%d", i);				
			}
			cell = cell->_next;
		}
	}
}


