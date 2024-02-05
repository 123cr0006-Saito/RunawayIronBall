#pragma once
#include "BuildingBase.h"

class Tower : public BuildingBase
{
public:
	Tower();
	~Tower();

private:
	
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
	std::vector<TOWER_PARTS_INFO> _partsInfo;
};