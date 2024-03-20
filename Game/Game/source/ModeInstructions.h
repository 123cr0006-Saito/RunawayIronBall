#pragma once
#include "appframe.h"
#include "dxlib.h"
#include <vector>
class ModeInstructions : public ModeBase
{
	typedef ModeBase base;
public :
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

protected:
	XInput* _input;
	//���ڂ̍ő吔
	static const int LIST_SIZE_MAX = 5;
	static const int _listViewMax = 5;//��ʂɕ\�������鍀�ڂ̍ő�T�C�Y

	int _listChoice;//���ڂ̒��ɂ���J�[�\���̈ʒu
	int _listViewNum;//��ʂɕ\�����鍀�ڂ̏ꏊ
	

	int _frameHandle;
	int _itemHandle[5];//�I�����ꂽ���ڂ̉摜
	int _imageHandle[5];//���ۂ̉�ʂ̉摜
};
