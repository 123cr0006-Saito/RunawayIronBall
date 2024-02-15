#pragma once
#include "appframe.h"
#include "AnimationItem.h"

#include <map>
#include <vector>

typedef std::map<int, ANIMATION_INFO> ANIM_MAP;
typedef std::string CHARA_NAME;

// �A�j���[�V�����Ǘ��N���X
class AnimationManager
{
public:
	

	AnimationManager();
	~AnimationManager();

	// �A�j���[�V�������p�}�b�v�R���e�i��ǉ�����
	// ����: map�R���e�i�ւ̃|�C���^�i�e�N���X�̐ÓI�����o�ϐ��j
	void InitMap(std::map<int, ANIMATION_INFO>* animMap);
	void InitMap(CHARA_NAME charaName, int modelHandle, std::string fileName);

	// ANIMATION_INFO�^�̃A�j���[�V�������̏����ݒ���s��
	void SetupAnimationInfo(int statusNo, int animIndex, int loopTimes);

	// �A�j���[�V�����A�C�e����ǉ�����
	void AddAnimationItem(int statusNo);

	// �A�j���[�V�����̍Đ�����
	void Process(int StatusNo);

	// �Ō�ɒǉ����ꂽ�A�j���[�V�����̍Đ����Ԃ��擾����
	float GetPlayTime() { return _playTime; }

private:
	static std::map<CHARA_NAME, ANIM_MAP> _animMap;

	// ���f���n���h��
	int _modelHandle;

	// �Đ����A�j���[�V�����̃X�e�[�^�X�ԍ�
	int _animNo;

	// �A�j���[�V�������p�}�b�v�R���e�i
	// �L�[ : int�^ �L�����N�^�[�̃X�e�[�^�X�ԍ�
	// �o�����[ : ANIMATION_INFO�^ �A�j���[�V�������
	ANIM_MAP* _charaAnimMapPtr;

	// �A�j���[�V�����A�C�e��
	std::vector<AnimationItem*> _animContainer;


	// �Ō�ɒǉ����ꂽ�̃A�j���[�V�����A�C�e��
	AnimationItem* _latestAnimItem;
	// �Ō�ɒǉ����ꂽ�A�j���[�V�����̍Đ�����
	float _playTime;

};