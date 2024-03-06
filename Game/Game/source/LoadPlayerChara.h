#pragma once
#include "dxlib.h"
#include "CharacterBase.h"
#include "AnimationManager.h"
#include "AnimationItem.h"
#include "Chain.h"
#include "FrameData.h"
#include "ModelColor.h"
class LoadPlayerChara
{
public:
	LoadPlayerChara();
	~LoadPlayerChara();

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

	Chain* _chain;

	ModelColor* _modelColor;
};

