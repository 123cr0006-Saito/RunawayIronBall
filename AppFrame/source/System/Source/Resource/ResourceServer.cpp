#define _CRT_SECURE_NO_WARNINGS 1
#include "../../Header/Resource/ResourceServer.h"

std::map<std::string, int> ResourceServer::handle_server;
std::map<std::string, int> ResourceServer::sound_server;
std::map<std::string, ResourceServer::Div> ResourceServer::div_server;
std::map<std::string, ResourceServer::Div> ResourceServer::mult_server;
std::map<std::string, int >ResourceServer::effekseer_server;
std::map<std::string, std::vector<int> >ResourceServer::model_server;


int ResourceServer::LoadGraph(std::string handle_name) {

	int value = 0;

	auto itr = handle_server.find(handle_name);
	if (itr != handle_server.end()) {
		//�L�^���ꂽ���̂����������̂Œl��Ԃ�
		value = itr->second;
	}
	else {
		//�L�^���ꂽ���O���Ȃ������̂œǂݍ���
		value = ::LoadGraph(handle_name.c_str());
		handle_server[handle_name] = value;
	}

	return value;
};

int ResourceServer::LoadSound(std::string sound_name) {

	int value = 0;

	auto itr = sound_server.find(sound_name);
	if (itr != sound_server.end()) {
		//�L�^���ꂽ���̂����������̂Œl��Ԃ�
		value = itr->second;
	}
	else {
		//�L�^���ꂽ���O���Ȃ������̂œǂݍ���
		value = ::LoadSoundMem(sound_name.c_str());
		sound_server[sound_name] = value;
	}

	return value;
};

int ResourceServer::LoadDivGraph(std::string handle_name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf) {

	int value = 0;
	auto itr = div_server.find(handle_name);

	if (itr != div_server.end()) {
		//�L�^���ꂽ���̂����������̂Œl��Ԃ�
		for (int i = 0; i < itr->second.AllNum; i++) {
			HandleBuf[i] = itr->second.handle[i];
		}
	}
	else {
		//�L�^���ꂽ���O���Ȃ������̂œǂݍ���
		//�ǂݍ��ޖ������킩��Ȃ����߃������𓮓I�m��
		int* buf = new int[AllNum];
		value = ::LoadDivGraph(handle_name.c_str(), AllNum, XNum, YNum, XSize, YSize, buf);

		if (value != -1) {
			//�G���[�ł͂Ȃ������ꍇ
			//�ő喇���Ɠǂݍ��݂񂾖������l���m��
			div_server[handle_name].AllNum = AllNum;
			div_server[handle_name].handle = buf;
			//�ǂݍ��񂾒l���ړ�
			//std::swap(HandleBuf, buf);
			for (int i = 0; i < AllNum; i++) {
				HandleBuf[i] = buf[i];
			}
		}
	}
	//�Ԃ��̂͐����������ꍇ��0 ���s�������ꍇ��-1
	return value;
};

int ResourceServer::LoadMultGraph(std::string handle_name, std::string extension, int AllNum, int* HandleBuf) {

	int value = 0;
	auto itr = mult_server.find(handle_name);

	if (itr != mult_server.end()) {
		//�L�^���ꂽ���̂����������̂Œl��Ԃ�
		for (int i = 0; i < itr->second.AllNum; i++) {
			HandleBuf[i] = itr->second.handle[i];
		}
	}
	else {
		//�L�^���ꂽ���O���Ȃ������̂œǂݍ���
		//�ǂݍ��ޖ������킩��Ȃ����߃������𓮓I�m��
		int* buf = new int[AllNum];
		char name[1024];
		std::string fileName = handle_name + " (%d)" + extension;
		for (int i = 1; i <= AllNum; i++) {
			std::sprintf(name, fileName.c_str(), i);
			buf[i - 1] = ::LoadGraph(name);
			if (buf[i - 1] == -1) {
				return -1;
			}
		}
		//�S�ēǂݍ���
		//�ő喇���Ɠǂݍ��݂񂾖������l���m��
		mult_server[handle_name].AllNum = AllNum;
		mult_server[handle_name].handle = buf;
		//�ǂݍ��񂾒l���ړ�

		for (int i = 0; i < AllNum; i++) {
			HandleBuf[i] = buf[i];
		}
	}
	//�Ԃ��̂͐����������ꍇ��0 ���s�������ꍇ��-1
	return value;
};

int ResourceServer::LoadEffekseerEffect(std::string handle_name) {

	int value = 0;

	auto itr = effekseer_server.find(handle_name);
	if (itr != effekseer_server.end()) {
		//�L�^���ꂽ���̂����������̂Œl��Ԃ�
		value = itr->second;
	}
	else {
		//�L�^���ꂽ���O���Ȃ������̂œǂݍ���
		value = ::LoadEffekseerEffect(handle_name.c_str());
		handle_server[handle_name] = value;
	}

	return value;
};

int ResourceServer::MV1LoadModel(std::string model_name) {
	int value = 0;

	auto itr = model_server.find(model_name);
	if (itr != model_server.end()) {
		//�L�^���ꂽ���̂����������̂ŃI���W�i�����R�s�[���ĕԂ�
		value = ::MV1DuplicateModel(itr->second.at(0));
		//��ō폜�ł���悤�ɔԍ��������Ă���
		model_server[model_name].push_back(value);
	}
	else {
		//�L�^���ꂽ���O���Ȃ������̂œǂݍ���
		value = ::MV1LoadModel(model_name.c_str());
		model_server[model_name].push_back(value);
	}

	return value;
};

int ResourceServer::SearchGraph(std::string serch_name) {
	//�摜�̌����@�������ꍇ�͉摜�̒l��Ԃ�
	auto itr = handle_server.find(serch_name);
	if (itr != handle_server.end()) {
		return itr->second;
	}
	return false;
};

void ResourceServer::DeleteResourceAll() {
	//�摜�̍폜
	for (auto itr = handle_server.begin(); itr != handle_server.end(); itr++) {
		DeleteGraph((*itr).second);
	}
	//���������摜�̍폜
	for (auto itr = div_server.begin(); itr != div_server.end(); itr++) {
		for (int i = 0; i < (*itr).second.AllNum; i++) {
			DeleteGraph((*itr).second.handle[i]);
		}
		delete[](*itr).second.handle;
	}
	//�ǂݍ��񂾕����̉摜�̍폜
	for (auto itr = mult_server.begin(); itr != mult_server.end(); itr++) {
		for (int i = 0; i < (*itr).second.AllNum; i++) {
			DeleteGraph((*itr).second.handle[i]);
		}
		delete[](*itr).second.handle;
	}
	//�G�t�F�N�V�A�̃G�t�F�N�g�̍폜
	for (auto itr = effekseer_server.begin(); itr != effekseer_server.end(); itr++) {
		DeleteEffekseerEffect((*itr).second);
	}
	//���f���̍폜
	for (auto itr = model_server.begin(); itr != model_server.end(); itr++) {
		for (int i = 0; i < itr->second.size(); i++) {
			MV1DeleteModel(itr->second.at(i));
		}
	}

	InitSoundMem();//�������͓ǂݍ��񂾂��̂��܂Ƃ߂ď�����̂ł��̊֐��ō폜����
	//�z��̍폜
	handle_server.clear();
	div_server.clear();
	effekseer_server.clear();
	model_server.clear();
	sound_server.clear();
};
