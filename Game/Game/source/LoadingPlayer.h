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

	VECTOR _pos;

	LoadingChain* _chain;

	ModelColor* _modelColor;
};

