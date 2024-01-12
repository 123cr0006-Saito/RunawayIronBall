#include "../../Header/Resource/ResourceServer.h"

std::map<std::string, int> ResourceServer::handle_server;
std::map<std::string, int> ResourceServer::sound_server;
std::map<std::string, ResourceServer::Div> ResourceServer::div_server;
std::map<std::string, int >ResourceServer::effekseer_server;
std::map<std::string, int >ResourceServer::model_server;

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

int ResourceServer::LoadEffekseerEffect(std::string handle_name) {

	int value = 0;

	auto itr = effekseer_server.find(handle_name);
	if (itr != effekseer_server.end()) {
		value = itr->second;
	}
	else {
		value = ::LoadEffekseerEffect(handle_name.c_str());
		effekseer_server[handle_name] = value;
	}

	return value;
};

int ResourceServer::MV1LoadModel(std::string model_name) {
	int value = 0;

	auto itr = model_server.find(model_name);
	if (itr != model_server.end()) {
		value = itr->second;
	}
	else {
		value = ::MV1LoadModel(model_name.c_str());
		model_server[model_name] = value;
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

int ResourceServer::SearchGraph(std::string serch_name) {
	auto itr = handle_server.find(serch_name);
	if (itr != handle_server.end()) {
		return itr->second;
	}
	return false;
};

void ResourceServer::DeleteResource() {
	for (auto itr = handle_server.begin(); itr != handle_server.end(); itr++) {
		DeleteGraph((*itr).second);
	}
	for (auto itr = div_server.begin(); itr != div_server.end(); itr++) {
		for (int i = 0; i < (*itr).second.AllNum; i++) {
			DeleteGraph((*itr).second.handle[i]);
		}
	}
	for (auto itr = effekseer_server.begin(); itr != effekseer_server.end(); itr++) {
		DeleteEffekseerEffect((*itr).second);
	}
	for (auto itr = model_server.begin(); itr != model_server.end(); itr++) {
		MV1DeleteModel((*itr).second);
	}

	InitSoundMem();
	handle_server.clear();
	div_server.clear();
	effekseer_server.clear();
	model_server.clear();
	sound_server.clear();
};
