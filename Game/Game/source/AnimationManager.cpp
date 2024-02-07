#include "AnimationManager.h"

AnimationManager::AnimationManager()
{
	_modelHandle = -1;
	_animNo = -1;
	_playTime = 0.0f;
}

AnimationManager::~AnimationManager()
{
}

// ANIMATION_INFO型のアニメーション情報の初期設定を行う
void AnimationManager::SetupAnimationInfo(int statusNo, int animIndex, int loopTimes)
{
	ANIMATION_INFO info;
	info.animIndex = animIndex;
	info.loopTimes = loopTimes;

	_animInfo[statusNo] = info;
}

// アニメーションアイテムを追加する
void AnimationManager::AddAnimationItem(int statusNo)
{
	auto itr = _animInfo.find(statusNo);
	// アニメーション情報が存在する場合
	if (itr != _animInfo.end())
	{
		AnimationItem* anim = new AnimationItem();

		ANIMATION_INFO info = itr->second;
		int attachIndex = MV1AttachAnim(_modelHandle, info.animIndex, -1, FALSE);
		float totalTime = MV1GetAttachAnimTotalTime(_modelHandle, attachIndex);
		int loopTimes = info.loopTimes;

		anim->Setup(attachIndex, totalTime, loopTimes);
	}
}

void AnimationManager::Process()
{
	for (auto itr = _anim.begin(); itr != _anim.end(); ++itr)
	{
	}
}
