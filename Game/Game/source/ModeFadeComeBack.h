//----------------------------------------------------------------------
// @filename ModeFadeComeBack.cpp
// ��date: 2024/03/02
// ��author: saito ko
// @explanation
// �X�e�[�W�J�ڎ��Ƀt�F�[�h�C���A�t�F�[�h�A�E�g���s�����߂̃N���X
// ModeFade�ƈႤ�_�͏�Ƀt�F�[�h�C�����Ă���t�F�[�h�A�E�g���A
// �w��̃��[�h���폜���Ȃ���J�ڂ���
//----------------------------------------------------------------------
#pragma once
#include "ModeFade.h"

class ModeFadeComeBack : public ModeFade
{
public:
	ModeFadeComeBack(int Time, std::string mode,int layer,bool IsProcessSkip = false); // ���C���[��ύX����ꍇ
	ModeFadeComeBack(int Time, ModeBase* mode, bool IsProcessSkip = false);  // ���[�h��delete����ꍇ
	ModeFadeComeBack(int Time, ModeBase* mode, std::string modeName, int layer ,bool IsProcessSkip = false);  //���C���[��ύX�� ���[�h��delete����ꍇ
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process()override;
	virtual bool Render()override;
protected:
	bool _IsProcessSkip;
	int _fadeEnd, _fadeStart;
	int _changeLayer;
	std::string _changeModeName;
	ModeBase* _deleteMode;
};

