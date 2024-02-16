#pragma once
#include "BuildingBase.h"
#include "TowerParts.h"

class Tower : public BuildingBase
{
	typedef BuildingBase base;

public:
	Tower();
	~Tower();

	//bool Init(int modelHandle, VECTOR startPos) override;
	bool Init(std::array<int, 3> modelHandle, VECTOR startPos);
	bool Process() override;
	bool Render() override;

	void BlastOffProcess();

	void SetFalling(VECTOR vDir);

	void UpdateCollision();

	// �f�o�b�O���̕\��
	bool DrawDebugInfo() override;

protected:
	
	struct TOWER_PARTS_INFO {
		// ���̗v�f���g�p�����ǂ���
		bool use;

		bool blast;
		VECTOR blastDir;

		// �p�[�c�̃��f���n���h��
		int modelHandle;
		// �p�[�c�̈ʒu
		VECTOR pos;
		// �e���ɂ�����1�t���[���̉�]�l
		VECTOR vRot;
	};

	// �p�[�c�̏��
	std::vector<TowerParts*> _towerParts;



	
	bool _isFalling;
	int _prevFallCnt;
	int _fallCnt;


	int _bottomIndex;
	VECTOR _endPos;

	float _r;
	float _up;
};