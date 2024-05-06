//----------------------------------------------------------------------
// @filename CollisionManager.h
// @date: 2024/03/07
// @author: Morozumi Hiroya
// 
// @explanation
// �����蔻�菈���̊Ǘ����s���N���X
// XZ���ʂɑ΂���4���؋�ԕ������s���A�����蔻�菈���̉񐔂��팸����
// 
// Cell�N���X��p���āA�I�u�W�F�N�g���Ǘ�����
// Cell��OBJ_TYPE�Ɋւ��ẮAObjectBase�N���X�̔h���N���X��Init�֐��Őݒ肷��
// Cell��OBJ_TYPE�ɂ��ObjectBase�N���X�̂ǂ̔h���N���X���̔��ʂ����̂ŁAObjectBase�N���X����e�h���N���X�ւ̃_�E���L���X�g��static_cast�ōs��
// 
// @reference
// �Q�l�T�C�g
// https://qiita.com/mogamoga1337/items/a1060d531b70c32a8ade
// http://marupeke296.com/COL_2D_No8_QuadTree.html
// http://marupeke296.com/COL_2D_No9_QuadTree_Imp.html
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "Cell.h"
#include "ObjectBase.h"

class CharacterBase;
class Player;
class IronBall;
class EnemyBase;
class BuildingBase;
class Tower;
class TowerParts;

// XZ���ʏ��4���؋�ԕ������s���A�����蔻����s���N���X
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	// �C���X�^���X���擾
	static CollisionManager* GetInstance() { return _instance; }

	// ����������
	void Init();
	// �X�V����
	void Process();

	// �Z�����c���[�֒ǉ��A�X�V
	void UpdateCell(Cell* cell);
	// �Z���̍폜�\��
	void ReserveRemovementCell(Cell* cell);

	// �c���[�ƃ��X�g�̃N���A
	// _tree, _colList, _reserveRemovementList���N���A����
	void ClearTreeAndList();

	// �f�o�b�O���̕\��
	// �Z�O�����g�̋�؂�����[���h��Ԃɕ`��
	void DrawSegment();
	// �Z����������Z�O�����g�̃C���f�b�N�X�����̃Z����ێ�����I�u�W�F�N�g�̍��W�ɕ`��
	void DrawSegmentIndex();

private:
	// 2�̒��_����Z����������c���[�̃C���f�b�N�X���v�Z
	unsigned int CalcTreeIndex(VECTOR pos1, VECTOR pos2);
	// �w�肳�ꂽ���W��������Z�O�����g�̃C���f�b�N�X���v�Z�i�ŏ��P�ʂ܂ŕ��������Z�O�����g�j
	unsigned int CalcSegment(VECTOR pos);
	// ���͒l��2�i���ň��΂��ɕ�������
	// �� : 1101 -> 01 01 00 01
	unsigned int SeparateBit(unsigned int n);

	// �c���[�ɃZ����ǉ�
	// �_�~�[�Z���̎��ɃZ����ǉ�����
	void InsertCellIntoTree(unsigned int treeIndex, Cell* cell);

	// �c���[����Z�����폜
	void RemoveCell(Cell* cell);
	// �폜�\�񃊃X�g�ɂ���Z�����폜
	void RemoveCellFromReserveList();

	// �����蔻�胊�X�g���쐬����
	void CreateColList(unsigned int treeIndex, std::list<Cell*>& colStack);
	// �����蔻�菈��
	void CheckColList();

	// --------------------------------------------------------------------------------------------
	// �����蔻�菈��
	void CheckHit(Player* player, EnemyBase* enemy);
	void CheckHit(Player* player, BuildingBase* building);
	void CheckHit(Player* player, Tower* tower);

	void CheckHitIbAndEn(IronBall* ironBall, EnemyBase* enemy);
	void CheckHitIbAndBldg(IronBall* ironBall, BuildingBase* building);
	void CheckHitIbAndTwr(IronBall* ironBall, Tower* tower);

	void CheckHitChAndEn(IronBall* ironBall, EnemyBase* enemy);
	void CheckHitChAndBldg(IronBall* ironBall, BuildingBase* building);
	void CheckHitChAndTwr(IronBall* ironBall, Tower* tower);

	void CheckHit(EnemyBase* enemy1, EnemyBase* enemy2);
	void CheckHit(EnemyBase* enemy, BuildingBase* building);
	void CheckHit(EnemyBase* enemy, Tower* tower);
	void CheckHit(EnemyBase* enemy, TowerParts* towerParts);

	void CheckHit(BuildingBase* building, TowerParts* towerParts);
	// --------------------------------------------------------------------------------------------

	// �C���X�^���X
	static CollisionManager* _instance;

	// ���[���h�̌��_����̃I�t�Z�b�g
	float _offsetX;
	float _offsetZ;

	// 1�ӂ�����̃Z�O�����g��
	int _segmentNumPerSide;
	// �ŏ��P�ʂ܂ŕ��������Z�O�����g��1�ӂ̒���
	float _segmentLength;

	// �c���[�̃T�C�Y
	// (pow(4, ������ + 1) - 1) / 3
	int _treeSize;
	// �c���[�i���`4���؁j
	// �e�Z�O�����g�ɑ����Ă���I�u�W�F�N�g��o�^����
	// �v�f���Ƃɑo�������X�g���\�z����
	// �e�v�f�̐擪�ɂ̓_�~�[�̃Z�����i�[���A��������ɂ̓I�u�W�F�N�g���i�[����
	std::vector<Cell*> _tree;

	// �����蔻�胊�X�g
	// �����蔻����s��2�Z���̃Z�����y�A�Ŋi�[����
	std::list<std::pair<Cell*, Cell*>> _colList;

	// �폜�\�񃊃X�g
	std::list<Cell*> _reserveRemovementList;
};