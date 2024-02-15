#pragma once
#include "BuildingBase.h"

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

	// �f�o�b�O���̕\��
	bool DrawDebugInfo() override;

protected:
	
	struct TOWER_PARTS_INFO {
		// ���̗v�f���g�p�����ǂ���
		bool use;
		// �p�[�c�̃��f���n���h��
		int modelHandle;
		// �p�[�c�̈ʒu
		VECTOR pos;
		// �e���ɂ�����1�t���[���̉�]�l
		VECTOR vRot;
	};

	// �p�[�c�̏��
	std::vector<TOWER_PARTS_INFO*> _partsInfo;
};