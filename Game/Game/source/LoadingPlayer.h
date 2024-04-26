//----------------------------------------------------------------------
// @filename LoadingPlayer.h
// @date: 2024/03/03
// @author: saito ko
// @explanation
// loading���̃L�����N�^�[�̂𐧌�E�`����s���N���X
//----------------------------------------------------------------------
#pragma once
#include "dxlib.h"
#include "CharacterBase.h"
#include "AnimationManager.h"
#include "AnimationItem.h"
#include "LoadingChain.h"
#include "FrameData.h"
#include "ModelColor.h"
class LoadingPlayer
{
public:
	LoadingPlayer();
	~LoadingPlayer();

	bool Process();
	bool Render();
	bool AnimationProcess();

	VECTOR GetPos() { return _pos; }

protected:

	int _modelHandle;

	int _attachIndex; // �A�j���[�V�����̃A�^�b�`�C���f�b�N�X
	int _totalTime; // �A�j���[�V�����̃g�[�^������
	float _playTime;// �A�j���[�V�����̌��݂̎���

	VECTOR _pos;// �v���C���[�̍��W

	LoadingChain* _chain;//���̃N���X

	ModelColor* _modelColor;// ���f���̐F��ύX����N���X
};

