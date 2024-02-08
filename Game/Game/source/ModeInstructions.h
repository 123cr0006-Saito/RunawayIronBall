#pragma once
#include "appframe.h"
#include "dxlib.h"
#include <vector>
#define NO_IMAGE 
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
	static const int LIST_SIZE_MAX = 20;
	static const int _listViewMax = 8;//��ʂɕ\�������鍀�ڂ̍ő�T�C�Y

	int _listChoice;//���ڂ̒��ɂ���J�[�\���̈ʒu
	int _listViewNum;//��ʂɕ\�����鍀�ڂ̏ꏊ
	
	struct SUMMARIZE{//�\�����������摜�������N������悤�ɂ��邽�߂̍\����
		int item;//�I�����ꂽ���ڂ̉摜
		int image;//���ۂ̉�ʂ̉摜
		int explanation;//�������̉摜
	};

	SUMMARIZE _handle[LIST_SIZE_MAX];//��������̍��ڂ̌�
};

