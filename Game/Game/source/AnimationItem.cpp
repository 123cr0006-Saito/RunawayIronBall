//----------------------------------------------------------------------
// @filename AnimationItem.cpp
// @date: 2024/02/07
// @author: Morozumi Hiroya
// @explanation
// �L�����N�^�[�̃A�j���[�V��������ێ�����N���X
// AnimationManager�N���X�ŁA���̃N���X�̃C���X�^���X��ǉ��E�폜���A�A�j���[�V�����̊Ǘ����s��
//----------------------------------------------------------------------
#include "AnimationItem.h"

AnimationItem::AnimationItem()
{
	_stateNo = -1;

	_attachIndex = -1;
	_totalTime = 0.0f;
	_playTime = 0.0f;

	_openTime = 0.0f;
	_openTotalTime = 0.0f;

	_closeTotalTime = 0.0f;
	_closeTime = 0.0f;

	_loopCnt = 0;
}

AnimationItem::~AnimationItem()
{
}

void AnimationItem::Setup(int animIndex, int attachIndex, float totalTime, int loopTimes)
{
	_stateNo = animIndex;

	_attachIndex = attachIndex;
	_totalTime = totalTime;
	_playTime = 0.0f;

	_openTime = 0.0f;
	_openTotalTime = 6.0f;

	_closeTotalTime = 0.0f;
	_closeTime = 0.0f;

	_loopCnt = loopTimes;
}
