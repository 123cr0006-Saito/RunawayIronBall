#include "CollisionManager.h"
#include "Suppression.h"
#include "Player.h"
#include "IronBall.h"
#include "EnemyBase.h"
#include "BuildingBase.h"
#include "Tower.h"
#include "TowerParts.h"

#include <math.h>

namespace {
	constexpr float STAGE_LENGTH = 27000.0f;
	constexpr int STAGE_DIVISION = 5;
}

// CellのOBJ_TYPEに関しては、ObjectBaseクラスの派生クラスのInit関数で設定する
// CellのOBJ_TYPEによりObjectBaseクラスのどの派生クラスかの判別がつくので、ObjectBaseクラスから各派生クラスへのダウンキャストはstatic_castで行う

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
	_colList.clear();
	_reserveRemovementList.clear();
}

CollisionManager::~CollisionManager()
{
	for (int i = 0; i < _treeSize; i++) {
		delete _tree[i];
	}
	_tree.clear();
	_colList.clear();
	_reserveRemovementList.clear();
}

void CollisionManager::Init()
{
	_offsetX = -STAGE_LENGTH / 2.0f;
	_offsetZ = -STAGE_LENGTH / 2.0f;
	_segmentNumPerSide = pow(2, STAGE_DIVISION);
	_segmentLength = STAGE_LENGTH / static_cast<float>(_segmentNumPerSide);


	_treeSize = (pow(4, STAGE_DIVISION + 1) - 1) / 3;
	_tree.resize(_treeSize);

	for (int i = 0; i < _treeSize; i++) {
		_tree[i] = NEW Cell();
	}
}

void CollisionManager::Process()
{
	// 削除予約リストにあるセルを削除
	// 各オブジェクトのProcess関数で当たり判定が無効化されたCellを削除する
	RemoveCellFromReserveList();

	_colList.clear();
	std::list<Cell*> colStack;
	CreateColList(0, colStack);
	CheckColList();

	// 削除予約リストにあるセルを削除
	// 当たり判定処理の終了後に当たり判定が無効化されたCellを削除する
	RemoveCellFromReserveList();
}

void CollisionManager::UpdateCell(Cell* cell)
{
	if (cell == nullptr || cell->_obj == nullptr) return;

	unsigned int treeIndex = 0;
	VECTOR pos1 = VGet(0.0f, 0.0f, 0.0f); // 最小座標
	VECTOR pos2 = VGet(0.0f, 0.0f, 0.0f); // 最大座標
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
	{
		IronBall* ironBall = static_cast<IronBall*>(cell->_obj);
		Sphere sphere = ironBall->GetIBCollision();
		pos1 = VGet(sphere.centerPos.x - sphere.r, 0.0f, sphere.centerPos.z - sphere.r);
		pos2 = VGet(sphere.centerPos.x + sphere.r, 0.0f, sphere.centerPos.z + sphere.r);
	}
	break;
	case OBJ_TYPE::PL_IB_CHAIN:
	{
		IronBall* ironBall = static_cast<IronBall*>(cell->_obj);
		Capsule capsule = ironBall->GetChainCollision();

		pos1 = capsule.up_pos;
		pos2 = capsule.up_pos;
		// 最小座標
		if (pos1.x > capsule.down_pos.x) pos1.x = capsule.down_pos.x;
		if (pos1.z > capsule.down_pos.z) pos1.z = capsule.down_pos.z;
		// 最大座標
		if (pos2.x < capsule.down_pos.x) pos2.x = capsule.down_pos.x;
		if (pos2.z < capsule.down_pos.z) pos2.z = capsule.down_pos.z;

		pos1 = VGet(pos1.x - capsule.r, 0.0f, pos1.z - capsule.r);
		pos2 = VGet(pos2.x + capsule.r, 0.0f, pos2.z + capsule.r);
	}
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

	case OBJ_TYPE::BLDG:
	{
		BuildingBase* building = static_cast<BuildingBase*>(cell->_obj);
		OBB obb = building->GetOBBCollision();

		std::vector<VECTOR> vertexPos;
		obb.GetVertexPos(vertexPos);

		pos1 = vertexPos[0];
		pos2 = vertexPos[0];
		for (int i = 1; i < 8; i++) {
			// 最小座標
			if (pos1.x > vertexPos[i].x) pos1.x = vertexPos[i].x;
			if (pos1.z > vertexPos[i].z) pos1.z = vertexPos[i].z;
			// 最大座標
			if (pos2.x < vertexPos[i].x) pos2.x = vertexPos[i].x;
			if (pos2.z < vertexPos[i].z) pos2.z = vertexPos[i].z;
		}

		pos1.y = 0.0f;
		pos2.y = 0.0f;
	}
	break;
	case OBJ_TYPE::TWR:
	{
		Tower* tower = static_cast<Tower*>(cell->_obj);
		Sphere sphere = tower->GetCollision();
		pos1 = VGet(sphere.centerPos.x - sphere.r, 0.0f, sphere.centerPos.z - sphere.r);
		pos2 = VGet(sphere.centerPos.x + sphere.r, 0.0f, sphere.centerPos.z + sphere.r);
	}
		break;
	case OBJ_TYPE::TWR_PRT:
	{
		TowerParts* towerParts = static_cast<TowerParts*>(cell->_obj);
		Sphere sphere = towerParts->GetCollision();
		pos1 = VGet(sphere.centerPos.x - sphere.r, 0.0f, sphere.centerPos.z - sphere.r);
		pos2 = VGet(sphere.centerPos.x + sphere.r, 0.0f, sphere.centerPos.z + sphere.r);
	}
	break;
	}

	treeIndex = CalcTreeIndex(pos1, pos2);
	if (cell->_segment != _tree[treeIndex]) {
		RemoveCell(cell);
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

void CollisionManager::RemoveCell(Cell* cell)
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

void CollisionManager::ReserveRemovementCell(Cell* cell)
{
	_reserveRemovementList.push_back(cell);
}

void CollisionManager::ClearTreeAndList()
{
	for (int i = 0; i < _treeSize; i++) {
		_tree[i]->_next = nullptr;
		_tree[i]->_prev = nullptr;
	}
	_colList.clear();
	_reserveRemovementList.clear();
}

void CollisionManager::RemoveCellFromReserveList()
{
	for (auto& cell : _reserveRemovementList) {
		RemoveCell(cell);
	}
	_reserveRemovementList.clear();
}

unsigned int CollisionManager::CheckArea(VECTOR pos)
{
	VECTOR p = VGet(pos.x - _offsetX, 0.0f, pos.z - _offsetZ);
	p.x = Math::Clamp(0.0f, STAGE_LENGTH - 1, p.x);
	p.z = Math::Clamp(0.0f, STAGE_LENGTH - 1, p.z);
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

		// オブジェクト1のタイプを判別
		switch (cell1->_objType) {
		case OBJ_TYPE::NONE:
			break;

			// オブジェクト1がプレイヤーの場合
		case OBJ_TYPE::PL:
		{
			Player* player = static_cast<Player*>(cell1->_obj);

			// オブジェクト2のタイプを判別
			switch (cell2->_objType) {
			case OBJ_TYPE::NONE:
				break;
			case OBJ_TYPE::EN:
			{
				EnemyBase* enemy = static_cast<EnemyBase*>(cell2->_obj);
				CheckHit(player, enemy);
			}
			break;
			case OBJ_TYPE::BLDG:
			{
				BuildingBase* building = static_cast<BuildingBase*>(cell2->_obj);
				CheckHit(player, building);
			}
			break;
			case OBJ_TYPE::TWR:
			{
				Tower* tower = static_cast<Tower*>(cell2->_obj);
				CheckHit(player, tower);
			}
			break;
			}
		}
		break; // end obj1 case OBJ_TYPE::PL

		// オブジェクト1がプレイヤーの鉄球の場合
		case OBJ_TYPE::PL_IB:
		{
			IronBall* ironBall = static_cast<IronBall*>(cell1->_obj);
			switch (cell2->_objType) {
			case OBJ_TYPE::NONE:
				break;
			case OBJ_TYPE::EN:
			{
				EnemyBase* enemy = static_cast<EnemyBase*>(cell2->_obj);
				CheckHitIbAndEn(ironBall, enemy);
			}
			break;
			case OBJ_TYPE::BLDG:
			{
				BuildingBase* building = static_cast<BuildingBase*>(cell2->_obj);
				CheckHitIbAndBldg(ironBall, building);
			}
			break;
			case OBJ_TYPE::TWR:
			{
				Tower* tower = static_cast<Tower*>(cell2->_obj);
				CheckHitIbAndTwr(ironBall, tower);
			}
			break;
			}
		}
		break; // end obj1 case OBJ_TYPE::PL_IB

		// オブジェクト1がプレイヤーの鉄球の鎖の場合
		case OBJ_TYPE::PL_IB_CHAIN:
		{
			IronBall* ironBall = static_cast<IronBall*>(cell1->_obj);
			switch (cell2->_objType) {
			case OBJ_TYPE::NONE:
				break;
			case OBJ_TYPE::EN:
			{
				EnemyBase* enemy = static_cast<EnemyBase*>(cell2->_obj);
				CheckHitChAndEn(ironBall, enemy);
			}
			break;
			case OBJ_TYPE::BLDG:
			{
				BuildingBase* building = static_cast<BuildingBase*>(cell2->_obj);
				CheckHitChAndBldg(ironBall, building);
			}
			break;
			case OBJ_TYPE::TWR:
			{
				Tower* tower = static_cast<Tower*>(cell2->_obj);
				CheckHitChAndTwr(ironBall, tower);
			}
			break;
			}
		}
		break; // end obj1 case OBJ_TYPE::PL_IB_CHAIN

		// オブジェクト1が敵の場合
		case OBJ_TYPE::EN:
		{
			EnemyBase* enemy1 = static_cast<EnemyBase*>(cell1->_obj);
			switch (cell2->_objType) {
			case OBJ_TYPE::NONE:
				break;
			case OBJ_TYPE::PL:
			{
				Player* player = static_cast<Player*>(cell2->_obj);
				CheckHit(player, enemy1);
			}
			break;
			case OBJ_TYPE::PL_IB:
			{
				IronBall* ironBall = static_cast<IronBall*>(cell2->_obj);
				CheckHitIbAndEn(ironBall, enemy1);
			}
			break;
			case OBJ_TYPE::PL_IB_CHAIN:
			{
				IronBall* ironBall = static_cast<IronBall*>(cell2->_obj);
				CheckHitChAndEn(ironBall, enemy1);
			}
			break;
			case OBJ_TYPE::EN:
			{
				EnemyBase* enemy2 = static_cast<EnemyBase*>(cell2->_obj);
				CheckHit(enemy1, enemy2);
			}
			break;
			case OBJ_TYPE::BLDG:
			{
				BuildingBase* building = static_cast<BuildingBase*>(cell2->_obj);
				CheckHit(enemy1, building);
			}
			break;
			case OBJ_TYPE::TWR:
			{
				Tower* tower = static_cast<Tower*>(cell2->_obj);
				CheckHit(enemy1, tower);
			}
			break;
			case OBJ_TYPE::TWR_PRT:
			{
				TowerParts* towerParts = static_cast<TowerParts*>(cell2->_obj);
				CheckHit(enemy1, towerParts);
			}
			break;
			}
		}
		break; // end obj1 case OBJ_TYPE::EN

		// オブジェクト1が建物の場合
		case OBJ_TYPE::BLDG:
		{
			BuildingBase* building = static_cast<BuildingBase*>(cell1->_obj);
			switch (cell2->_objType) {
			case OBJ_TYPE::NONE:
				break;
			case OBJ_TYPE::PL:
			{
				Player* player = static_cast<Player*>(cell2->_obj);
				CheckHit(player, building);
			}
			break;
			case OBJ_TYPE::PL_IB:
			{
				IronBall* ironBall = static_cast<IronBall*>(cell2->_obj);
				CheckHitIbAndBldg(ironBall, building);
			}
			break;
			case OBJ_TYPE::PL_IB_CHAIN:
			{
				IronBall* ironBall = static_cast<IronBall*>(cell2->_obj);
				CheckHitChAndBldg(ironBall, building);
			}
			break;
			case OBJ_TYPE::EN:
			{
				EnemyBase* enemy = static_cast<EnemyBase*>(cell2->_obj);
				CheckHit(enemy, building);
			}
			break;
			case OBJ_TYPE::TWR_PRT:
			{
				TowerParts* towerParts = static_cast<TowerParts*>(cell2->_obj);
				CheckHit(building, towerParts);
			}
			break;
			}
		}
		break; // end obj1 case OBJ_TYPE::BLDG

		// オブジェクト1がタワー本体の場合
		case OBJ_TYPE::TWR:
		{
			Tower* tower = static_cast<Tower*>(cell1->_obj);
			switch (cell2->_objType) {
			case OBJ_TYPE::NONE:
				break;
			case OBJ_TYPE::PL:
			{
				Player* player = static_cast<Player*>(cell2->_obj);
				CheckHit(player, tower);
			}
			break;
			case OBJ_TYPE::PL_IB:
			{
				IronBall* ironBall = static_cast<IronBall*>(cell2->_obj);
				CheckHitIbAndTwr(ironBall, tower);
			}
			break;
			case OBJ_TYPE::PL_IB_CHAIN:
			{
				IronBall* ironBall = static_cast<IronBall*>(cell2->_obj);
				CheckHitChAndTwr(ironBall, tower);
			}
			break;
			case OBJ_TYPE::EN:
			{
				EnemyBase* enemy = static_cast<EnemyBase*>(cell2->_obj);
				CheckHit(enemy, tower);
			}
			break;
			}
		}
		break; // end obj1 case OBJ_TYPE::TWR

		// オブジェクト1がタワーのパーツの場合
		case OBJ_TYPE::TWR_PRT:
		{
			TowerParts* towerParts = static_cast<TowerParts*>(cell1->_obj);
			switch (cell2->_objType) {
			case OBJ_TYPE::NONE:
				break;
			case OBJ_TYPE::EN:
			{
				EnemyBase* enemy = static_cast<EnemyBase*>(cell2->_obj);
				CheckHit(enemy, towerParts);
			}
			break;
			case OBJ_TYPE::BLDG:
			{
				BuildingBase* building = static_cast<BuildingBase*>(cell2->_obj);
				CheckHit(building, towerParts);
			}
			break;
			}
		}
		break; // end obj1 case OBJ_TYPE::TWR_PRT
		}
	}
}
void CollisionManager::CheckHit(Player* player, EnemyBase* enemy)
{
	// 敵が撃破ノックバック状態の時は当たり判定を行わない
	if (enemy->GetEnemyState() == ENEMYTYPE::DEAD) return;
	Capsule pCol = player->GetCollision();
	Sphere eCol = { enemy->GetCollisionPos(), enemy->GetR() };

	if (Collision3D::SphereCapsuleCol(eCol, pCol)) {
		bool isInvincible = player->GetIsInvincible();
		if (!isInvincible) {
			player->SetDamage();
		}
		VECTOR tmpPos = enemy->GetCollisionPos();
		tmpPos.y = 0.0f;

		VECTOR vDir = VSub(pCol.down_pos, tmpPos);
		vDir.y = 0.0f;
		float squareLength = VSquareSize(vDir);
		if (squareLength >= 0.0001f) {
			vDir = VNorm(vDir);
			tmpPos = VAdd(tmpPos, VScale(vDir, eCol.r + pCol.r));
			player->SetPos(tmpPos);
		}
	}
}

void CollisionManager::CheckHit(Player* player, BuildingBase* building)
{
	Capsule pCol = player->GetCollision();
	OBB bCol = building->GetOBBCollision();
	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);

	if (Collision3D::OBBCapsuleCol(bCol, pCol, &hitPos)) {
		VECTOR vDir = VSub(pCol.down_pos, hitPos);
		vDir.y = 0.0f;
		vDir = VNorm(vDir);
		hitPos.y = 0.0f;
		VECTOR tmpPos = VAdd(hitPos, VScale(vDir, pCol.r));
		player->SetPos(tmpPos);
	}
}

void CollisionManager::CheckHit(Player* player, Tower* tower)
{
	// XZ平面上に投影した円と円の当たり判定
	Capsule pCol = player->GetCollision();
	Sphere tCol = tower->GetCollision();
	pCol.down_pos.y = 0.0f;
	tCol.centerPos.y = 0.0f;
	VECTOR vSub = VSub(pCol.down_pos, tCol.centerPos);
	float sqLength = VSquareSize(vSub);
	if (sqLength < (pCol.r + tCol.r) * (pCol.r + tCol.r)) {
		float length = sqrt(sqLength);
		VECTOR vDir = VScale(vSub, 1.0f / length);
		VECTOR tmpPos = VAdd(tCol.centerPos, VScale(vDir, tCol.r + pCol.r + 1.0f));
		player->SetPos(tmpPos);
	}
}

void CollisionManager::CheckHitIbAndEn(IronBall* ironBall, EnemyBase* enemy)
{
	bool enabledIBAttackCollision = ironBall->GetEnabledAttackCollision();
	if (enabledIBAttackCollision) {
		Sphere ibCol = ironBall->GetIBCollision();
		Sphere eCol = { enemy->GetCollisionPos(), enemy->GetR() };

		if (Collision3D::SphereCol(ibCol.centerPos, ibCol.r, eCol.centerPos, eCol.r)) {
			ObjectBase* obj = ironBall->GetParentInstance();
			Player* player = static_cast<Player*>(obj);

			VECTOR pPos = player->GetPosition();
			VECTOR vDir = VSub(eCol.centerPos, pPos);
			vDir = VNorm(vDir);
			enemy->SetKnockBackAndDamage(vDir, player->GetPower());
			global._soundServer->DirectPlay("SE_Hit01");
		}
	}
}

void CollisionManager::CheckHitIbAndBldg(IronBall* ironBall, BuildingBase* building)
{
	bool enabledIBAttackCollision = ironBall->GetEnabledAttackCollision();
	if (building->GetUseCollision() && building->GetCanBreak() && enabledIBAttackCollision) {
		Sphere ibCol = ironBall->GetIBCollision();
		OBB bCol = building->GetOBBCollision();

		if (Collision3D::OBBSphereCol(bCol, ibCol)) {
			Player* player = static_cast<Player*>(ironBall->GetParentInstance());
			VECTOR vDir = VSub(bCol.pos, player->GetPosition());
			building->SetHit(vDir);
			player->SetExp(building->GetExp());
			Suppression::GetInstance()->SubSuppression(building->GetSuppression());
			global._soundServer->DirectPlay(building->GetName() + "_Break");
		}
	}
}

void CollisionManager::CheckHitIbAndTwr(IronBall* ironBall, Tower* tower)
{
	bool enabledIBAttackCollision = ironBall->GetEnabledAttackCollision();
	bool canBlast = tower->GetCanBlast();
	if (enabledIBAttackCollision && canBlast) {
		Sphere ibCol = ironBall->GetIBCollision();
		Sphere tCol = tower->GetCollision();

		if (Collision3D::SphereCol(ibCol, tCol)) {
			Player* player = static_cast<Player*>(ironBall->GetParentInstance());
			VECTOR vDir = VSub(tCol.centerPos, player->GetPosition());
			player->SetExp(50);
			tower->SetBlast(vDir);
			global._soundServer->DirectPlay("SE_Hit_Tower");
		}
	}
}

void CollisionManager::CheckHitChAndEn(IronBall* ironBall, EnemyBase* enemy)
{
	bool enabledIBAttackCollision = ironBall->GetEnabledAttackCollision();
	if (enabledIBAttackCollision) {
		Capsule cCol = ironBall->GetChainCollision();
		Sphere eCol = { enemy->GetCollisionPos(), enemy->GetR() };

		if (Collision3D::SphereCapsuleCol(eCol, cCol)) {
			ObjectBase* obj = ironBall->GetParentInstance();
			Player* player = static_cast<Player*>(obj);

			VECTOR pPos = player->GetPosition();
			VECTOR vDir = VSub(eCol.centerPos, pPos);
			vDir = VNorm(vDir);
			enemy->SetKnockBackAndDamage(vDir, player->GetPower());
			global._soundServer->DirectPlay("SE_Hit01");
		}
	}
}

void CollisionManager::CheckHitChAndBldg(IronBall* ironBall, BuildingBase* building)
{
	bool enabledIBAttackCollision = ironBall->GetEnabledAttackCollision();
	if (building->GetUseCollision() && building->GetCanBreak() && enabledIBAttackCollision) {
		Capsule cCol = ironBall->GetChainCollision();
		OBB bCol = building->GetOBBCollision();

		if (Collision3D::OBBCapsuleCol(bCol, cCol)) {
			Player* player = static_cast<Player*>(ironBall->GetParentInstance());
			VECTOR vDir = VSub(bCol.pos, player->GetPosition());
			building->SetHit(vDir);
			player->SetExp(building->GetExp());
			global._soundServer->DirectPlay(building->GetName() + "_Break");
			Suppression::GetInstance()->SubSuppression(building->GetSuppression());
		}
	}
}

void CollisionManager::CheckHitChAndTwr(IronBall* ironBall, Tower* tower)
{
	bool enabledIBAttackCollision = ironBall->GetEnabledAttackCollision();
	bool canBlast = tower->GetCanBlast();
	if (enabledIBAttackCollision && canBlast) {
		Capsule cCol = ironBall->GetChainCollision();
		Sphere tCol = tower->GetCollision();

		if (Collision3D::SphereCapsuleCol(tCol, cCol)) {
			Player* player = static_cast<Player*>(ironBall->GetParentInstance());
			VECTOR vDir = VSub(tCol.centerPos, player->GetPosition());
			player->SetExp(50);
			tower->SetBlast(vDir);
			global._soundServer->DirectPlay("SE_Hit_Tower");
		}
	}
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

void CollisionManager::CheckHit(EnemyBase* enemy, BuildingBase* building)
{
	// エネミーが撃破ノックバック状態、かつ、建物が破壊不可である場合は処理を行わない
	if (enemy->GetEnemyState() == ENEMYTYPE::DEAD && building->GetCanBreak() == false) return;
	Sphere eCol = { enemy->GetCollisionPos(), enemy->GetR() };
	OBB bCol = building->GetOBBCollision();
	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);

	if (Collision3D::OBBSphereCol(bCol, eCol, &hitPos)) {
		// エネミーが撃破ノックバック状態の時、建物にぶつかったら破壊する
		if (enemy->GetEnemyState() == ENEMYTYPE::DEAD) {
			VECTOR vDir = VSub(bCol.pos, eCol.centerPos);
			building->SetHit(vDir);
			Player::GetInstance()->SetExp(building->GetExp());
			global._soundServer->DirectPlay(building->GetName() + "_Break");
			Suppression::GetInstance()->SubSuppression(building->GetSuppression());
		}
		else {
			// 敵の押し出し処理
			VECTOR vDir = VSub(eCol.centerPos, hitPos);
			vDir.y = 0.0f;
			vDir = VNorm(vDir);

			hitPos.y = 0.0f;
			VECTOR tmpPos = VAdd(hitPos, VScale(vDir, eCol.r));
			enemy->SetPos(tmpPos);
		}
	}
}

void CollisionManager::CheckHit(EnemyBase* enemy, Tower* tower)
{
	// XZ平面上に投影した円と円の当たり判定
	Sphere eCol = { enemy->GetCollisionPos(), enemy->GetR() };
	Sphere tCol = tower->GetCollision();
	eCol.centerPos.y = 0.0f;
	tCol.centerPos.y = 0.0f;
	VECTOR vSub = VSub(eCol.centerPos, tCol.centerPos);
	float sqLength = VSquareSize(vSub);
	if (sqLength < (eCol.r + tCol.r) * (eCol.r + tCol.r)) {
		float length = sqrt(sqLength);
		VECTOR vDir = VScale(vSub, 1.0f / length);
		VECTOR tmpPos = VAdd(tCol.centerPos, VScale(vDir, tCol.r + eCol.r + 1.0f));
		enemy->SetPos(tmpPos);
	}
}

void CollisionManager::CheckHit(EnemyBase* enemy, TowerParts* towerParts)
{
	bool isBlast = towerParts->GetIsBlast();
	if (isBlast) {
		Sphere eCol = { enemy->GetCollisionPos(), enemy->GetR() };
		Sphere tCol = towerParts->GetCollision();

		if (Collision3D::SphereCol(eCol, tCol)) {
			VECTOR vDir = VSub(eCol.centerPos, tCol.centerPos);
			vDir = VNorm(vDir);
			//float r = rand() % 2;
			//r = 1;
			//vDir = VTransform(vDir, MGetRotY(Math::DegToRad(45.0f * r)));
			enemy->SetKnockBackAndDamage(vDir, 9999);
			global._soundServer->DirectPlay("SE_Hit01");
		}
	}
}

void CollisionManager::CheckHit(BuildingBase* building, TowerParts* towerParts)
{
	bool isBlast = towerParts->GetIsBlast();
	if (isBlast) {
		OBB bCol = building->GetOBBCollision();
		Sphere tCol = towerParts->GetCollision();

		if (Collision3D::OBBSphereCol(bCol, tCol)) {
			VECTOR vDir = VSub(bCol.pos, tCol.centerPos);
			building->SetHit(vDir);
			Player::GetInstance()->SetExp(building->GetExp());
			global._soundServer->DirectPlay(building->GetName() + "_Break");
			Suppression::GetInstance()->SubSuppression(building->GetSuppression());
		}
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
	for (int i = 0; i < _segmentNumPerSide + 1; i++) {
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

			VECTOR worldPos = VGet(0.0f, 0.0f, 0.0f);
			ObjectBase* obj = nullptr;
			switch (cell->_objType)
			{
			case OBJ_TYPE::NONE:
				break;
			case OBJ_TYPE::PL:
				obj = cell->_obj;
				worldPos = static_cast<Player*>(obj)->GetPosition();
				break;
			case OBJ_TYPE::PL_IB:
				obj = cell->_obj;
				worldPos = static_cast<IronBall*>(obj)->GetBallPosition();
				break;
			case OBJ_TYPE::PL_IB_CHAIN:
				obj = cell->_obj;
				worldPos = static_cast<IronBall*>(obj)->GetChainCollision().up_pos;
				break;
			case OBJ_TYPE::EN:
				obj = cell->_obj;
				worldPos = static_cast<EnemyBase*>(obj)->GetCollisionPos();
				break;
			case OBJ_TYPE::BLDG:
				obj = cell->_obj;
				worldPos = static_cast<BuildingBase*>(obj)->GetPos();
				break;
			case OBJ_TYPE::TWR:
				obj = cell->_obj;
				worldPos = static_cast<Tower*>(obj)->GetPos();
				break;
			case OBJ_TYPE::TWR_PRT:
				obj = cell->_obj;
				worldPos = static_cast<TowerParts*>(obj)->GetPos();
				break;
			}
			VECTOR screenPos = ConvWorldPosToScreenPos(worldPos);
			if (0.0f < screenPos.z && screenPos.z < 1.0f) {
				DrawFormatString(screenPos.x, screenPos.y, COLOR_RED, "%d", i);
			}
			cell = cell->_next;
		}
	}
}


