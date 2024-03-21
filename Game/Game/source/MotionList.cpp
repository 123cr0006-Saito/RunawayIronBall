#include "MotionList.h"
#include "appframe.h"

std::map<CHARA_NAME, std::vector<MotionNamePair>> MotionList::_allCharaMotionList;

MotionList::MotionList()
{
}

MotionList::~MotionList()
{
}

void MotionList::Load(CHARA_NAME charaName, std::string fileName)
{
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

void MotionList::DeleteAll()
{
	_allCharaMotionList.clear();
}

std::vector<MotionNamePair>* MotionList::GetMotionList(CHARA_NAME charaName)
{
	auto itr = _allCharaMotionList.find(charaName);
	if (itr != _allCharaMotionList.end()) {
		return &itr->second;
	}

	return nullptr;
}
