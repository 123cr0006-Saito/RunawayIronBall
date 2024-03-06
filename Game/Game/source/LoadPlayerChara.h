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

	int _attachIndex; // アニメーションのアタッチインデックス
	int _totalTime; // アニメーションのトータル時間
	float _playTime;// アニメーションの現在の時間

	VECTOR _pos;

	Chain* _chain;

	ModelColor* _modelColor;
};

