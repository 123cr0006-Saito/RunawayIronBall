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
	XInput* _input;// ���̓N���X
	int _gaussHandle;// �K�E�X�̉摜
	int _selectItem;// �I�𒆂̍���
	int _bgmVolum;// BGM�̉���
	int _seVolum;// SE�̉���
	static const int MAX_MODE = 5;// ���ڂ̍ő吔
	VERTEX2D _seGauge[4];// SE�̃Q�[�W�̒��_�f�[�^
	VERTEX2D _bgmGauge[4];// BGM�̃Q�[�W�̒��_�f�[�^
	static const unsigned short vertex[6];// �|���S����`�悷��ۂ̒��_�̏���
	std::unordered_map<std::string, int> _handleMap;// �摜�n���h���̊Ǘ��R���e�i
};