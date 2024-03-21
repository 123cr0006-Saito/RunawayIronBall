#pragma once

// �A�j���[�V�������
struct ANIMATION_INFO {
	// �A�j���[�V�����̃C���f�b�N�X�ԍ�
	int animIndex;
	// ���[�v�񐔁i0�Ȃ疳�����[�v�j
	int loopTimes;
};

// �A�j���[�V�����A�C�e���N���X
class AnimationItem
{
private:
	friend class AnimationManager;
	AnimationItem();
	~AnimationItem();

	// �A�j���[�V�����A�C�e���̏����ݒ�
	void Setup(int animIndex, int attachIndex, float totalTime, int loopTimes);

	int _stateNo;

	// �A�j���[�V�����̃A�^�b�`�C���f�b�N�X
	int _attachIndex;
	// �A�j���[�V�����̑��Đ�����
	float _totalTime;
	// �A�j���[�V�����̍Đ�����
	float _playTime;

	// ���[�V�����u�����h�p
	float _closeTotalTime;
	float _closeTime;
	float _openTotalTime;
	float _openTime;
	

	// ���[�v�J�E���g�p
	int _loopCnt;
};