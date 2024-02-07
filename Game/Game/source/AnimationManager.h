#pragma once
#include "appframe.h"
#include "AnimationItem.h"

#include <map>
#include <vector>

// �A�j���[�V�������
struct ANIMATION_INFO {
	// �A�j���[�V�����̃C���f�b�N�X�ԍ�
	int _animIndex;
	// ���[�v�񐔁i0�Ȃ疳�����[�v�j
	int _loopTimes;
};


class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();

	// ANIMATION_INFO�^�̃A�j���[�V��������ǉ�����
	void SetupAnimationInfo(int statusNo, int animIndex, int loopTimes);

private:
	// ���f���n���h��
	int _modelHandle;

	// �Đ����̃A�j���[�V�����ԍ�
	int _animNo;

	// �A�j���[�V�������p�}�b�v�R���e�i
	// �L�[ : int�^ �L�����N�^�[�̃X�e�[�^�X�ԍ�
	// �o�����[ : ANIMATION_INFO�^ �A�j���[�V�������
	std::map<int, ANIMATION_INFO> _animInfo;

	// �A�j���[�V�����A�C�e��
	std::vector<AnimationItem*> _anim;

	// �A�j���[�V�����̍Đ�����
	float _playTime;
};