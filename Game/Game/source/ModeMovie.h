//----------------------------------------------------------------------
// @filename ModeMovie.h
// @date: 2024/03/16
// @author: saito ko
// @explanation
// �Q�[���N���A���ɃG���f�B���O���[�r�[���Đ�����N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class ModeMovie : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
protected:
	int _movie;// ���[�r�[�n���h��
};

