#include "../../Header/Resource/ResourceServer.h"
ResourceServer::ResourceServer() {

};

ResourceServer::~ResourceServer() {
	for (auto itr = handle_server.begin(); itr != handle_server.end();itr++) {
		DeleteGraph((*itr).second);
	}
	for (auto itr = div_server.begin(); itr != div_server.end(); itr++) {
		for (int i = 0; i < (*itr).second.AllNum; i++) {
			DeleteGraph((*itr).second.handle[i]);
		}
	}
	InitSoundMem();
	handle_server.clear();
	div_server.clear();
	sound_server.clear();
};

int ResourceServer::LoadGraph(std::string handle_name) {

	int value = 0;

	auto itr = handle_server.find(handle_name);
	if (itr != handle_server.end()) {
		value = itr->second;
	}
	else {
		value = ::LoadGraph(handle_name.c_str());
		handle_server[handle_name] = value;
	}

	return value;
};

int ResourceServer::LoadDivGraph(std::string handle_name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf) {

	int value = 0;
	auto itr = div_server.find(handle_name);

	if (itr != div_server.end()) {
		for (int i = 0; i < itr->second.AllNum; i++) {
			HandleBuf[i] = itr->second.handle[i];
		}
	}
	else {
		int* buf = new int[AllNum];
		value = ::LoadDivGraph(handle_name.c_str(), AllNum, XNum, YNum, XSize, YSize, buf);

		if (value != -1) {
			div_server[handle_name].AllNum = AllNum;
			div_server[handle_name].handle = buf;
			for (int i = 0; i < AllNum; i++) {
				HandleBuf[i] = buf[i];
			}
		}
	}

	return value;
};

int ResourceServer::LoadSound(std::string sound_name) {

	int value = 0;

	auto itr = sound_server.find(sound_name);
	if (itr != sound_server.end()) {
		value = itr->second;
	}
	else {
		value = ::LoadSoundMem(sound_name.c_str());
		sound_server[sound_name] = value;
	}

	return value;
};

int ResourceServer::SearchGraph(std::string serch_name) {
	auto itr = handle_server.find(serch_name);
	if (itr != handle_server.end()) {
		return itr->second;
	}
	return false;
};
