//----------------------------------------------------------------------
// @filename ModeTutorial.h
// @date: 2024/03/17
// @author: saito ko
// @explanation
// �X�e�[�W�̍ŏ��Ƀ`���[�g���A����ʂ�\������N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class ModeTutorial : public ModeBase
{
	typedef ModeBase base;
public:
	ModeTutorial(int* handle,int size);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
protected:
	XInput* _input;// ���̓N���X
	int* _handle;// �摜�n���h���̔z��
	int _pageSize;// �摜�̖���
	int _selectItem;// �`�撆�̍��ڔԍ�
};