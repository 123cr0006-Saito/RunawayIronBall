//----------------------------------------------------------------------
// @filename ModePause.h
// ��date: 2023/12/25
// ��author: saito ko
// @explanation
// ���ʐݒ��U���ݒ�A���������ʁA�Q�[���I����I������|�[�Y��ʂ̃N���X
//----------------------------------------------------------------------
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

	void SetVertex();

	void SelectSetVolum(int& setVolum);//�T�E���h�̐ݒ�
	void SelectSetVibration();//�R���g���[���[�̐U���ݒ�
	void SelectOperationInstructions();//����������
	void SelectGameEnd();//�Q�[�����I������

	void VertexProcess();

protected:
	XInput* _input;
	int gauss_handle;
	int _selectItem;
	//-----------------------------
	//�����O���[�o���ō�邩�A���ꂼ��̃N���X�𒼐ڃI�v�V�����ł����邩
	int _bgmVolum;
	int _seVolum;
	static const int MAX_MODE = 5;

	VERTEX2D _seGauge[4];
	VERTEX2D _bgmGauge[4];

	static const unsigned short vertex[6];

	std::unordered_map<std::string, int> _handleMap;
};