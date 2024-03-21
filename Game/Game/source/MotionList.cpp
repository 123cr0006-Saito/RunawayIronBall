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

		// csvファイルを読み込む
		CFile file(filePath);
		// ファイルが開けた場合
		if (file.Success()) {
			const char* data = (const char*)file.Data();
			int c = 0;
			int size = file.Size();
			int stateNo = 0;
			while (c < size) {
				std::string motionName;
				int loopTimes = 0;
				// モーション名を取得
				c += GetString(&data[c], &motionName);
				// カンマの位置を見つける
				c += FindString(&data[c], ',', &data[size]); c++;
				// ループ回数を取得
				c += GetDecNum(&data[c], &loopTimes);
				// 改行などスキップ
				c += SkipSpace(&data[c], &data[size]);

				MotionNamePair m = { motionName, loopTimes };
				_allCharaMotionList[charaName].push_back(m);
			}
		}
#ifdef _DEBUG
		else {
			// ファイルが開けなかった場合
			MessageBox(NULL, "ファイルが開けませんでした", "エラー", MB_OK);
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
