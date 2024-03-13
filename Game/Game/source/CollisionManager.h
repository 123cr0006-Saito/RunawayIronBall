#pragma once
#include "appframe.h"
#include "Cell.h"
#include "ObjectBase.h"
class CharacterBase;
class Player;
class EnemyBase;
class BuildingBase;
class IronBall;

// XZ���ʏ��4���؋�ԕ������s���A�����蔻����s���N���X
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	static CollisionManager* GetInstance() { return _instance; }

	void Init();
	void Process();

	// �c���[�փZ����ǉ��A�X�V
	void UpdateCell(Cell* cell);
	// �c���[����Z�����폜
	void RemoveCell(Cell* cell);

	// �Z���̍폜�\��
	void ReserveRemovementCell(Cell* cell);
	// �폜�\�񃊃X�g�ɂ���Z�����폜
	void RemoveCellFromReserveList();


	// �f�o�b�O���̕\��
	void DrawSegmentLine();
	void DrawAreaIndex();

private:
	unsigned int CalcTreeIndex(VECTOR pos1, VECTOR pos2);
	unsigned int CheckArea(VECTOR pos);
	unsigned int SeparateBit(unsigned int n);
	void InsertCellIntoTree(unsigned int treeIndex, Cell* cell);
	void CreateColList(unsigned int treeIndex, std::list<Cell*>& colStack);
	void CheckColList();

	// �����蔻�菈��
	void CheckHit(Player* player, EnemyBase* enemy);
	void CheckHit(Player* player, BuildingBase* building);

	void CheckHitIbAndEn(IronBall* ironBall, EnemyBase* enemy);
	void CheckHitIbAndBldg(IronBall* ironBall, BuildingBase* building);

	void CheckHitChAndEn(IronBall* ironBall, EnemyBase* enemy);

	void CheckHit(EnemyBase* enemy1, EnemyBase* enemy2);
	void CheckHit(EnemyBase* enemy, BuildingBase* building);

	static CollisionManager* _instance;

	float _offsetX;
	float _offsetZ;

	int _segmentNumPerSide;
	float _segmentLength;


	int _treeSize;
	std::vector<Cell*> _tree;

	// �����蔻����s���Z���̃��X�g
	std::list<std::pair<Cell*, Cell*>> _colList;

	// �폜�\�񃊃X�g
	std::list<Cell*> _reserveRemovementList;
};