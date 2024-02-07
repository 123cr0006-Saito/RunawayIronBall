#include "AnimationManager.h"

AnimationManager::AnimationManager()
{
	_playTime = 0.0f;
}

AnimationManager::~AnimationManager()
{
}

// ANIMATION_INFO�^�̃A�j���[�V��������ǉ�����
void AnimationManager::SetupAnimationInfo(int statusNo, int animIndex, int loopTimes)
{
	ANIMATION_INFO* info;
	info->statusNo = statusNo;
	info->animIndex = animIndex;
	info->loopTimes = loopTimes;
	_animInfo.push_back(info);
}
