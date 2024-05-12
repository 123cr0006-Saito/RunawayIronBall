//----------------------------------------------------------------------
// @filename MotionList.cpp
// @date: 2024/02/20
// @author: Morozumi Hiroya
// @explanation
// �e�L�����N�^�[�̃��[�V�������X�g���Ǘ�����N���X
// AnimationManager�N���X�y��FrameData�N���X�̏����������ŕK�v�ȏ����Ǘ�����
// "Data/MotionList/MotionList_�Z�Z.csv" ����A�e�L�����N�^�[�̃��[�V�����ꗗ��ǂݍ��ށi�����̕����ɂ̓L�����N�^�[��������j
//----------------------------------------------------------------------
#include "MotionList.h"
#include "appframe.h"

std::map<CHARA_NAME, std::vector<MotionNamePair>> MotionList::_allCharaMotionList;

// ���[�V�������X�g�̓ǂݍ���
// @param charaName: �L�����N�^�[��
// @param fileName: ���[�V�������X�g�̃t�@�C����
void MotionList::Load(CHARA_NAME charaName, std::string fileName)
{
	// ���łɊY������L�����N�^�[�̃��[�V�������X�g�����݂���ꍇ�͓ǂݍ��܂Ȃ�
	auto itr = _allCharaMotionList.find(charaName);
	if (itr == _allCharaMotionList.end()) {
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

				MotionNamePair m = { motionName, loopTimes };
				_allCharaMotionList[charaName].push_back(m);
			}
		}
#ifdef _DEBUG
		else {
			// �t�@�C�����J���Ȃ������ꍇ
			MessageBox(NULL, "�t�@�C�����J���܂���ł���", "�G���[", MB_OK);
		}
#endif // _DEBUG
	}
}

// ���[�V�������X�g�����ׂč폜����
void MotionList::DeleteAll()
{
	_allCharaMotionList.clear();
}

// �w�肵���L�����N�^�[�̃��[�V�������X�g���擾����
// @param charaName: �L�����N�^�[��
std::vector<MotionNamePair>* MotionList::GetMotionList(CHARA_NAME charaName)
{
	// �w�肵���L�����N�^�[�̃��[�V�������X�g�����݂���ꍇ
	auto itr = _allCharaMotionList.find(charaName);
	if (itr != _allCharaMotionList.end()) {
		return &itr->second;
	}
	// ���݂��Ȃ��ꍇ
	return nullptr;
}
