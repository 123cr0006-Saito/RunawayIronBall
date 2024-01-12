#include "../../Header/Resource/ResourceServer.h"

std::map<std::string, int> ResourceServer::handle_server;
std::map<std::string, int> ResourceServer::sound_server;
std::map<std::string, ResourceServer::Div> ResourceServer::div_server;
std::map<std::string, int >ResourceServer::effekseer_server;
std::map<std::string, int >ResourceServer::model_server;

int ResourceServer::LoadResource(const std::string resource_name, std::map<std::string, int>* server_name) {

	int value = 0;

	auto itr = (*server_name).find(resource_name);
	if (itr != (*server_name).end()) {
		value = itr->second;
	}
	else {
		value = ::LoadGraph(resource_name.c_str());
		(*server_name)[resource_name] = value;
	}

	return value;
};

int ResourceServer::LoadGraph(std::string handle_name) {
	return LoadResource(handle_name, &handle_server);
};

int ResourceServer::LoadEffekseerEffect(std::string handle_name) {
	return LoadResource(handle_name, &effekseer_server);
};

int ResourceServer::MV1LoadModel(std::string model_name) {
	return LoadResource(model_name, &model_server);
};

int ResourceServer::LoadSound(std::string sound_name) {
	return LoadResource(sound_name, &sound_server);
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
