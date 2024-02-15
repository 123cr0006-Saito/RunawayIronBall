#include "AnimationManager.h"

std::map<CHARA_NAME, ANIM_MAP> AnimationManager::_animMap;

AnimationManager::AnimationManager()
{
	_modelHandle = -1;
	_animNo = -1;
	_playTime = 0.0f;
	_charaAnimMapPtr = nullptr;
	_latestAnimItem = nullptr;
}

AnimationManager::~AnimationManager()
{
	for (auto itr = _animContainer.begin(); itr != _animContainer.end(); ++itr)
	{
		delete (*itr);
		(*itr) = nullptr;
	}
	_animContainer.clear();
	_charaAnimMapPtr = nullptr;
}

// �A�j���[�V�������p�}�b�v�R���e�i��ǉ�����
//	����: map�R���e�i�ւ̃|�C���^�i�e�N���X�̐ÓI�����o�ϐ��j
void AnimationManager::InitMap(std::map<int, ANIMATION_INFO>* animMap)
{
	if (this->_charaAnimMapPtr == nullptr) {
		this->_charaAnimMapPtr = animMap;
	}
}

void AnimationManager::InitMap(CHARA_NAME charaName, int modelHandle, std::string fileName)
{
	auto itr = _animMap.find(charaName);
	if (itr == _animMap.end()) {
		std::string filePath = "Data/MotionList/" + fileName;

		// csv�t�@�C����ǂݍ���
		CFile file(filePath);
		// �t�@�C�����J�����ꍇ
		if (file.Success()) {
			const char* data = (const char*)file.Data();
			int c = 0;
			int size = file.Size();
			int stateNo = 0;
			while (c < size) {
				std::string motionName;
				int loopTimes = 0;
				// ���[�V���������擾
				c += GetString(&data[c], &motionName);
				// �J���}�̈ʒu��������
				c += FindString(&data[c], ',', &data[size]); c++;
				// ���[�v�񐔂��擾
				c += GetDecNum(&data[c], &loopTimes);
				// ���s�ȂǃX�L�b�v
				c += SkipSpace(&data[c], &data[size]);

				ANIMATION_INFO info;
				info.animIndex = MV1GetAnimIndex(modelHandle, motionName.c_str());
				info.loopTimes = loopTimes;
				_animMap[charaName][stateNo] = info;
				stateNo++;
#ifdef _DEBUG
				if (info.animIndex == -1) {
					std::string message = "[" + motionName + "] �A�j���[�V������������܂���ł���";

					MessageBox(NULL, message.c_str(), "�G���[", MB_OK);
				}
#endif // _DEBUG
			}
		}
#ifdef _DEBUG
		else {
			// �t�@�C�����J���Ȃ������ꍇ
			MessageBox(NULL, "�t�@�C�����J���܂���ł���", "�G���[", MB_OK);
		}
#endif // _DEBUG
	}
	_charaAnimMapPtr = &_animMap[charaName];
	_modelHandle = modelHandle;
}

// ANIMATION_INFO�^�̃A�j���[�V�������̏����ݒ���s��
void AnimationManager::SetupAnimationInfo(int statusNo, int animIndex, int loopTimes)
{
	// ����statusNo�ɑΉ�����A�j���[�V������񂪑��݂��邩���ׂ�
	auto itr = (*_charaAnimMapPtr).find(statusNo);
	// �A�j���[�V������񂪑��݂��Ȃ��ꍇ�̂݁A�A�j���[�V��������ǉ�����
	if (itr == (*_charaAnimMapPtr).end()) {
		ANIMATION_INFO info;
		info.animIndex = animIndex;
		info.loopTimes = loopTimes;
		(*_charaAnimMapPtr)[statusNo] = info;
	}

}

// �A�j���[�V�����A�C�e����ǉ�����
void AnimationManager::AddAnimationItem(int statusNo)
{
	auto itr = (*_charaAnimMapPtr).find(statusNo);
	// �A�j���[�V������񂪑��݂���ꍇ
	if (itr != (*_charaAnimMapPtr).end())
	{
		AnimationItem* anim = new AnimationItem();

		ANIMATION_INFO info = itr->second;
		int attachIndex = MV1AttachAnim(_modelHandle, info.animIndex, -1, FALSE);
		float totalTime = MV1GetAttachAnimTotalTime(_modelHandle, attachIndex);
		int loopTimes = info.loopTimes;

		anim->Setup(attachIndex, totalTime, loopTimes);
		_animContainer.push_back(anim);
		_latestAnimItem = anim;
	}
}

// �A�j���[�V�����̍Đ�����
void AnimationManager::Process(int StatusNo)
{
	// �Đ�����A�j���[�V�������ς�����ꍇ
	if (_animNo != StatusNo) {
		// �A�^�b�`����Ă���A�j���[�V�����ɕ����Ԃ�ݒ肷��
		for (auto itrItem = _animContainer.begin(); itrItem != _animContainer.end(); ++itrItem)
		{
			if ((*itrItem)->_closeTime == 0.0f) {
				// �����Ԃ�ݒ肷��
				(*itrItem)->_closeTotalTime = 4.0f;
				(*itrItem)->_closeTime = (*itrItem)->_closeTotalTime;
			}
		}

		// �A�j���[�V�����ԍ����X�V����
		_animNo = StatusNo;
		// �A�j���[�V�����A�C�e����ǉ�����
		AddAnimationItem(StatusNo);
	}

	// �Ō�ɒǉ����ꂽ�A�j���[�V�����A�C�e���̍Đ����Ԃ��擾����
	_playTime = _latestAnimItem->_playTime;

	for (auto itrItem = _animContainer.begin(); itrItem != _animContainer.end(); )
	{
		// �Đ����Ԃ��Z�b�g����
		MV1SetAttachAnimTime(_modelHandle, (*itrItem)->_attachIndex, (*itrItem)->_playTime);

		/* �Đ����Ԃ̍X�V���� */
		// �����Ԃ��ݒ肳��Ă��Ȃ��ꍇ
		if ((*itrItem)->_closeTime == 0.0f) {
			// �Đ����Ԃ�i�߂�
			(*itrItem)->_playTime += 1.0f;
			
			// �Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�
			if ((*itrItem)->_playTime > (*itrItem)->_totalTime) {
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
				itrItem = _animContainer.erase(itrItem);
				continue;
			}
			// �u�����h����ύX����
			MV1SetAttachAnimBlendRate(_modelHandle, (*itrItem)->_attachIndex, (*itrItem)->_closeTime / (*itrItem)->_closeTotalTime);
		}
		// ���̃A�j���[�V�����A�C�e����
		++itrItem;
	}
}
