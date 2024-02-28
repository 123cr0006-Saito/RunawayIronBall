#pragma once
#include "appframe.h"
#include "ModeInstructions.h"
#include "ModeTitle.h"
#include <array>

class ModePause : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void SelectSetVolum(int& setVolum);//�T�E���h�̐ݒ�
	void SelectSetVibration();//�R���g���[���[�̐U���ݒ�
	void SelectOperationInstructions();//����������
	void SelectGameEnd();//�Q�[�����I������

protected:
	XInput* _input;
	int gauss_handle;
	int _selectItem;
	//-----------------------------
	//�����O���[�o���ō�邩�A���ꂼ��̃N���X�𒼐ڃI�v�V�����ł����邩
	int _bgmVolum;
	int _seVolum;
	bool _isVibration;
	static const int MAX_MODE = 5;

	std::unordered_map<std::string, int> _handleMap;
};