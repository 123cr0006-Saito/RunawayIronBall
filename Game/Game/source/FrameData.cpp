#include "FrameData.h"
FrameData::FrameData(){

};

FrameData::~FrameData() {

};

bool FrameData::LoadData(std::string, std::vector<std::pair<int, std::string>> frameData) {
	return true;
};

void FrameData::Process(int state, int animTime) {
	//コンテナの初期化
	if (!_nextCommandList.empty()) {
		_nextCommandList.clear();
	}

	auto itr = _frameData[state].equal_range(animTime);
	for (auto& it = itr.first; it != itr.second ; ++it) {
		if (it->second.first < 1000) {
			//キャラ固有の処理
			_nextCommandList.push_back(it->second);
		}
		else {
			//エフェクト
		}
	}
};

