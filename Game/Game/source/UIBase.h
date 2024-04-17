//----------------------------------------------------------------------
// @filename UIBase.h
// ��date: 2023/12/19
// ��author: saito ko
// @explanation
// UI�̊��N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "dxlib.h"
#include <string>
//��ʂɕ\������UI�̊�ՂƂȂ�N���X
class UIBase
{
public:
	UIBase(VECTOR pos);
	UIBase(VECTOR pos, int size, int* handle);
	virtual ~UIBase();
	virtual bool Process();
	virtual bool Draw();

protected:
	int* _handle;// �摜�n���h��
	int _handleNum;// �摜�n���h���̐�
	VECTOR _pos;// �ʒu
	float _cx, _cy;// ���S���W
};
