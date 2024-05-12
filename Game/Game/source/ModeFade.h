//----------------------------------------------------------------------
// @filename ModeFade.h
// @date: 2024/02/29
// @author: saito ko
// @explanation
// �X�e�[�W�J�ڎ��Ƀt�F�[�h�C���A�t�F�[�h�A�E�g���s�����߂̃N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"

class ModeFade : public ModeBase
{
protected: 
	typedef ModeBase base;
public:
	ModeFade(int Time,bool FadeIn = false);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process()override;
	virtual bool Render()override;
protected:
	int _alphaFade;// �t�F�[�h�̃A���t�@�l
	int _currentTime;// ���݂̎���
	int _fadeTime;// �t�F�[�h���鎞��
    bool _isFadeIn;// �t�F�[�h�C�����t�F�[�h�A�E�g��
};

