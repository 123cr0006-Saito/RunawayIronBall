#include "FrameData.h"
FrameData::FrameData(){

};

FrameData::~FrameData() {

};

bool LoadData(std::string, std::vector<std::pair<int, std::string>> frameData) {
	return true;
};

void FrameData::Process(int state, int animTime) {
	auto itr = _frameData[state].equal_range(animTime);
	for (auto& it = itr.first; it != itr.second ; ++it) {
		if (it->second.first < 1000) {
			//�L�����ŗL�̏���
			_nextCommandList.push_back(it->second);
		}
		else {
			//�G�t�F�N�g
		}
	}
};

