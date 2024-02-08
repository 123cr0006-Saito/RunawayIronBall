#include "AnimationManager.h"

AnimationManager::AnimationManager(int modelHandle)
{
	_modelHandle = modelHandle;
	_animNo = -1;
	_playTime = 0.0f;
}

AnimationManager::~AnimationManager()
{
}

// ANIMATION_INFO�^�̃A�j���[�V�������̏����ݒ���s��
void AnimationManager::SetupAnimationInfo(int statusNo, int animIndex, int loopTimes)
{
	ANIMATION_INFO info;
	info.animIndex = animIndex;
	info.loopTimes = loopTimes;

	_animInfo[statusNo] = info;
}

// �A�j���[�V�����A�C�e����ǉ�����
void AnimationManager::AddAnimationItem(int statusNo)
{
	auto itr = _animInfo.find(statusNo);
	// �A�j���[�V������񂪑��݂���ꍇ
	if (itr != _animInfo.end())
	{
		AnimationItem* anim = new AnimationItem();

		ANIMATION_INFO info = itr->second;
		int attachIndex = MV1AttachAnim(_modelHandle, info.animIndex, -1, FALSE);
		float totalTime = MV1GetAttachAnimTotalTime(_modelHandle, attachIndex);
		int loopTimes = info.loopTimes;

		anim->Setup(attachIndex, totalTime, loopTimes);
		_anim.push_back(anim);
	}
}

void AnimationManager::Process(int StatusNo)
{
	// �Đ�����A�j���[�V�������ς�����ꍇ
	if (_animNo != StatusNo) {
		// �A�^�b�`����Ă���A�j���[�V�����ɕ����Ԃ�ݒ肷��
		for (auto itrItem = _anim.begin(); itrItem != _anim.end(); ++itrItem)
		{
			if ((*itrItem)->_closeTime == 0.0f) {
				// �����Ԃ�ݒ肷��
				(*itrItem)->_closeTotalTime = 6.0f;
				(*itrItem)->_closeTime = (*itrItem)->_closeTotalTime;
			}
		}

		// �A�j���[�V�����ԍ����X�V����
		_animNo = StatusNo;
		// �A�j���[�V�����A�C�e����ǉ�����
		AddAnimationItem(StatusNo);
	}

	for (auto itrItem = _anim.begin(); itrItem != _anim.end(); )
	{
		if ((*itrItem)->_closeTime == 0.0f) {
			// �Đ����Ԃ�i�߂�
			(*itrItem)->_playTime += 1.0f;
			
			// �Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�
			if ((*itrItem)->_playTime >= (*itrItem)->_totalTime) {
				if ((*itrItem)->_loopCnt > 1 || (*itrItem)->_loopCnt == 0) {
					if ((*itrItem)->_loopCnt > 1) {
						(*itrItem)->_loopCnt--;
					}
					// �Đ����Ԃ�0�ɖ߂�
					(*itrItem)->_playTime = 0.0f;
				}
				else {
					(*itrItem)->_playTime = (*itrItem)->_totalTime;
				}
			}
		}
		else {
			// �����Ԃ����炷
			(*itrItem)->_closeTime -= 1.f;

			// �����Ԃ��I��������H
			if ((*itrItem)->_closeTime <= 0.f) {
				// �A�j���[�V�������f�^�b�`����
				MV1DetachAnim(_modelHandle, (*itrItem)->_attachIndex);
				// ���̃A�j���[�V�������폜
				delete (*itrItem);
				itrItem = _anim.erase(itrItem);
				continue;
			}
			// �u�����h����ύX����
			MV1SetAttachAnimBlendRate(_modelHandle, (*itrItem)->_attachIndex, (*itrItem)->_closeTime / (*itrItem)->_closeTotalTime);
		}
		// �Đ����Ԃ��Z�b�g����
		MV1SetAttachAnimTime(_modelHandle, (*itrItem)->_attachIndex, (*itrItem)->_playTime);
		// ���̃A�j���[�V�����A�C�e����
		++itrItem;
	}
}
