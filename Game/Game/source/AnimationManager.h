#pragma once
#include "appframe.h"
#include "AnimationItem.h"

#include <map>
#include <vector>


// �A�j���[�V�����Ǘ��N���X
class AnimationManager
{
public:
	AnimationManager(int modelHandle);
	~AnimationManager();

	// �A�j���[�V�������p�}�b�v�R���e�i��ǉ�����
	// ����: map�R���e�i�ւ̃|�C���^�i�e�N���X�̐ÓI�����o�ϐ��j
	void InitMap(std::map<int, ANIMATION_INFO>* animMap);

	// ANIMATION_INFO�^�̃A�j���[�V�������̏����ݒ���s��
	void SetupAnimationInfo(int statusNo, int animIndex, int loopTimes);

	// �A�j���[�V�����A�C�e����ǉ�����
	void AddAnimationItem(int statusNo);

	// �A�j���[�V�����̍Đ�����
	void Process(int StatusNo);

private:
	// ���f���n���h��
	int _modelHandle;

	// �Đ����A�j���[�V�����̃X�e�[�^�X�ԍ�
	int _animNo;

	// �A�j���[�V�������p�}�b�v�R���e�i
	// �L�[ : int�^ �L�����N�^�[�̃X�e�[�^�X�ԍ�
	// �o�����[ : ANIMATION_INFO�^ �A�j���[�V�������
	std::map<int, ANIMATION_INFO>* _animMap;

	// �A�j���[�V�����A�C�e��
	std::vector<AnimationItem*> _animContainer;

	// �A�j���[�V�����̍Đ�����
	float _playTime;

};