//----------------------------------------------------------------------
// @filename Heart.h
// @date: 2024/03/03
// @author: saito ko
// @explanation
// �v���C���[���G�ꂽ��g�o���񕜂���I�u�W�F�N�g�N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class Heart
{
public :
	Heart(VECTOR pos);
	~Heart();
	bool Process();
	bool Render();

	Sphere GetSphere() { return _sphere; }
protected:
	int _model;//���f���n���h��
	int _currentTime;//���݂̎���
	VECTOR _pos;//���W
	Sphere _sphere;//�����蔻��
};

