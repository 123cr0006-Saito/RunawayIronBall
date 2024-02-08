#pragma once
#include "appframe.h"
#include "AnimationItem.h"

#include <map>
#include <vector>



class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();

	// ANIMATION_INFO�^�̃A�j���[�V�������̏����ݒ���s��
	void SetupAnimationInfo(int statusNo, int animIndex, int loopTimes);

	// �A�j���[�V�����A�C�e����ǉ�����
	void AddAnimationItem(int statusNo);

	// �A�j���[�V�����̍Đ�����
	void Process();

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