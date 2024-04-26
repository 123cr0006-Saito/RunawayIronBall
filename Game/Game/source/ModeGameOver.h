//----------------------------------------------------------------------
// @filename ModeGameOver.h
// @date: 2024/02/15
// @author: saito ko
// @explanation
// �Q�[���I�[�o�[���ɑJ�ڂ��A���g���C���^�C�g���ɖ߂邩��I�����郂�[�h
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class ModeGameOver : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
	void AnimProcess();
	void SelectProcess();
protected:
	XInput* _input;// ���̓N���X
	std::unordered_map<std::string,int> _handle;// �摜�n���h���̊Ǘ��R���e�i
	int _selectItem;// �I�𒆂̍���

	bool _selectEnd;// �I���I��
	int _model;// ���f���n���h��
	int _modelFrame;// ���f���̃t���[��
	VECTOR _targetPos;// �J�����̃^�[�Q�b�g���W
	VECTOR _cameraPos;// �J�����̈ʒu
};

