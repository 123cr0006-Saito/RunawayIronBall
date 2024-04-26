//----------------------------------------------------------------------
// @filename CrystarRoof.h
// @date: 2024/02/24
// @author: saito ko
// @explanation
// �N���C�X�^�[�̉���������`�悷�邽�߂̃N���X
//----------------------------------------------------------------------
#pragma once
#include "DxLib.h"
#include "appframe.h"
class CrystarRoof
{
public:
	CrystarRoof(int model, int parent,std::string frameName);
	~CrystarRoof();
	bool Update();
	bool Render();
protected:
	int _model;//���f���n���h��
	int _frame;//�t���[���̔ԍ�
	int _parent;//�e�̃��f���n���h��
	std::string _joint;//�e�̃��f���̂ǂ̃W���C���g�ɂ������邩
};

