#include "CollisionManager.h"

#include "EnemyBase.h"


#include <math.h>

namespace {
	constexpr float STAGE_LENGTH = 27000.0f;
	constexpr int STAGE_DIVISION = 5;
}

CollisionManager* CollisionManager::_instance = nullptr;

CollisionManager::CollisionManager()
{
#ifdef _DEBUG
	if (_instance != nullptr) {
		MessageBox(0, "CollisionManagerクラスは既に生成されています", "エラー", MB_OK);
}
#endif // _DEBUG

	_instance = this;
	_offsetX = 0.0f;
	_offsetZ = 0.0f;
	_segmentNumPerSide = 0;
	_segmentLength = 0.0f;
	_treeSize = 0;
	_tree.clear();
}

CollisionManager::~CollisionManager()
{
	for (int i = 0; i < _treeSize; i++) {
		delete _tree[i];
	}
	_tree.clear();
	_colList.clear();
}

void CollisionManager::Init()
{
	_offsetX = -STAGE_LENGTH / 2.0f;
	_offsetZ = -STAGE_LENGTH / 2.0f;
	_segmentNumPerSide = pow(2, STAGE_DIVISION);
	_segmentLength = STAGE_LENGTH / static_cast<float>(_segmentNumPerSide);


	_treeSize = (pow(4, STAGE_DIVISION + 1) - 1) / 3;
	_tree.resize(_treeSize);

	for(int i = 0; i < _treeSize; i++) {
		_tree[i] = new Cell();
	}
}

void CollisionManager::Process()
{
	_colList.clear();
	std::list<Cell*> colStack;
	CreateColList(0, colStack);

	CheckColList();
}

void CollisionManager::UpdateCell(Cell* cell)
{
	if(cell == nullptr || cell->_obj == nullptr) return;

	unsigned int treeIndex = 0;
	VECTOR pos1 = VGet(0.0f, 0.0f, 0.0f);
	VECTOR pos2 = VGet(0.0f, 0.0f, 0.0f);
	switch (cell->_objType)
	{
	case OBJ_TYPE::NONE:
		return;
		break;
	case OBJ_TYPE::PL:
	{
		Player* player = static_cast<Player*>(cell->_obj);
		Capsule capsule = player->GetCollision();
		pos1 = VGet(capsule.up_pos.x - capsule.r, 0.0f, capsule.up_pos.z - capsule.r);
		pos2 = VGet(capsule.up_pos.x + capsule.r, 0.0f, capsule.up_pos.z + capsule.r);
	}
		break;
	case OBJ_TYPE::PL_IB:
		break;
	case OBJ_TYPE::EN:
	{
		EnemyBase* enemy = static_cast<EnemyBase*>(cell->_obj);
		VECTOR centerPos = enemy->GetCollisionPos();
		float r = enemy->GetR();
		pos1 = VGet(centerPos.x - r, 0.0f, centerPos.z - r);
		pos2 = VGet(centerPos.x + r, 0.0f, centerPos.z + r);
	}
		break;
	case OBJ_TYPE::EN_IB:
		break;
	}

	treeIndex = CalcTreeIndex(pos1, pos2);
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
			unsigned int bit = xorIndex & 0b11;
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

void CollisionManager::CheckColList()
{
	for (auto& colPair : _colList) {
		Cell* cell1 = colPair.first;
		Cell* cell2 = colPair.second;
		if (cell1->_obj == nullptr || cell2->_obj == nullptr) continue;

		switch (cell1->_objType) {
		case OBJ_TYPE::EN:
		{
			EnemyBase* enemy1 = static_cast<EnemyBase*>(cell1->_obj);
			switch (cell2->_objType) {
			case OBJ_TYPE::EN:
			{
				EnemyBase* enemy2 = static_cast<EnemyBase*>(cell2->_obj);
				CheckHit(enemy1, enemy2);
			}
			break;
			}
		}
		break;
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

void CollisionManager::CheckHit(EnemyBase* enemy1, EnemyBase* enemy2)
{
	VECTOR en1Pos = enemy1->GetCollisionPos();
	float en1R = enemy1->GetR();
	VECTOR en2Pos = enemy2->GetCollisionPos();
	float en2R = enemy2->GetR();

	VECTOR vDir = VSub(en2Pos, en1Pos);
	float sqLength = VSquareSize(vDir);
	if (sqLength < (en1R + en2R) * (en1R + en2R)) {
		float length = sqrt(sqLength);
		vDir = VScale(vDir, 1.0f / length);
		enemy2->SetExtrusionPos(VScale(vDir, (en1R + en2R) - length));
	}
}

void CollisionManager::CreateColList(unsigned int treeIndex, std::list<Cell*>& colStack)
{
	Cell* cell1 = _tree[treeIndex]->_next;
	// ① 同空間内のオブジェクト同士の衝突リストを作成
	while (cell1 != nullptr)
	{
		Cell* cell2 = cell1->_next;
		while (cell2 != nullptr)
		{
			_colList.push_back(std::make_pair(cell1, cell2));
			cell2 = cell2->_next;
		}
		// ② 衝突スタックとの衝突リストを作成
		for (auto& colCell : colStack) {
			_colList.push_back(std::make_pair(cell1, colCell));
		}
		cell1 = cell1->_next;
	}

	bool childFlag = false;
	// ③ 子空間を調べる
	unsigned int objNum = 0;
	// 1つの空間は4つの子空間を持つ
	for (int j = 0; j < 4; j++) {
		unsigned int childIndex = treeIndex * 4 + 1 + j;
		if (childIndex < _treeSize && _tree[childIndex] != nullptr) {
			if (!childFlag) {
				// ④ treeIndexのCellをスタックに追加
				cell1 = _tree[treeIndex]->_next;
				while (cell1 != nullptr) {
					colStack.push_back(cell1);
					objNum++;
					cell1 = cell1->_next;
				}
			}
			childFlag = true;
			CreateColList(childIndex, colStack);
		}
	}

	// ⑤ スタックからCellを取り除く
	if (childFlag) {
		for (int i = 0; i < objNum; i++) {
			colStack.pop_back();
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
	for (int i = 0; i < _treeSize; i++) {
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


