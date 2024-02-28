#define _CRT_SECURE_NO_WARNINGS 1
#include "../../Header/Resource/ResourceServer.h"

std::map<const char*, int> ResourceServer::_handleMap;
std::map<const char*, int> ResourceServer::_soundMap;
std::map<const char*, ResourceServer::Mult> ResourceServer::_multMap;
std::map<const char*, int >ResourceServer::_effekseerMap;
std::map<const char*, std::vector<int> >ResourceServer::_modelMap;


int ResourceServer::LoadGraph(const char* key_name, const char* handle_name) {

	int value = 0;

	auto itr = _handleMap.find(key_name);
	if (itr != _handleMap.end()) {
		//記録されたものが見つかったので値を返す
		value = itr->second;
	}
	else {
		//記録された名前がなかったので読み込み
		value = ::LoadGraph(handle_name);
		_handleMap[key_name] = value;
	}

	return value;
};

int ResourceServer::LoadSound(const char* key_name, const char* sound_name) {

	int value = 0;

	auto itr = _soundMap.find(key_name);
	if (itr != _soundMap.end()) {
		//記録されたものが見つかったので値を返す
		value = itr->second;
	}
	else {
		//記録された名前がなかったので読み込み
		value = ::LoadSoundMem(sound_name);
		_soundMap[key_name] = value;
	}

	return value;
};

int ResourceServer::LoadDivGraph(const char* key_name, const char* handle_name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf) {

	int value = 0;
	auto itr = _multMap.find(key_name);

	if (itr != _multMap.end()) {
		//記録されたものが見つかったので値を返す
		for (int i = 0; i < itr->second.AllNum; i++) {
			HandleBuf[i] = itr->second.handle[i];
		}
	}
	else {
		//記録された名前がなかったので読み込み
		//読み込む枚数がわからないためメモリを動的確保
		int* buf = new int[AllNum];
		value = ::LoadDivGraph(handle_name, AllNum, XNum, YNum, XSize, YSize, buf);

		if (value != -1) {
			//エラーではなかった場合
			//最大枚数と読み込みんだ枚数分値を確保
			_multMap[key_name].AllNum = AllNum;
			_multMap[key_name].handle = buf;
			//読み込んだ値を移動
			//std::swap(HandleBuf, buf);
			for (int i = 0; i < AllNum; i++) {
				HandleBuf[i] = buf[i];
			}
		}
	}
	//返すのは成功だった場合の0 失敗だった場合の-1
	return value;
};

int ResourceServer::LoadMultGraph(const char* key_name, std::string handle_name, const char* extension, int AllNum, int* HandleBuf) {

	int value = 0;
	auto itr = _multMap.find(key_name);

	if (itr != _multMap.end()) {
		//記録されたものが見つかったので値を返す
		for (int i = 0; i < itr->second.AllNum; i++) {
			HandleBuf[i] = itr->second.handle[i];
		}
	}
	else {
		//記録された名前がなかったので読み込み
		//読み込む枚数がわからないためメモリを動的確保
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
		//全て読み込んだ
		//最大枚数と読み込みんだ枚数分値を確保
		_multMap[key_name].AllNum = AllNum;
		_multMap[key_name].handle = buf;
		//読み込んだ値を移動

		for (int i = 0; i < AllNum; i++) {
			HandleBuf[i] = buf[i];
		}
	}
	//返すのは成功だった場合の0 失敗だった場合の-1
	return value;
};

int ResourceServer::LoadEffekseerEffect(const char* key_name, const char* handle_name) {

	int value = 0;

	auto itr = _effekseerMap.find(key_name);
	if (itr != _effekseerMap.end()) {
		//記録されたものが見つかったので値を返す
		value = itr->second;
	}
	else {
		//記録された名前がなかったので読み込み
		value = ::LoadEffekseerEffect(handle_name);
		_handleMap[key_name] = value;
	}

	return value;
};

int ResourceServer::MV1LoadModel(const char* key_name, const char* model_name) {
	int value = 0;

	auto itr = _modelMap.find(key_name);
	if (itr != _modelMap.end()) {
		//記録されたものが見つかったのでオリジナルをコピーして返す
		value = ::MV1DuplicateModel(itr->second.at(0));
		//後で削除できるように番号も持っておく
		_modelMap[key_name].push_back(value);
	}
	else {
		//記録された名前がなかったので読み込み
		value = ::MV1LoadModel(model_name);
		_modelMap[key_name].push_back(value);
	}

	return value;
};

std::pair<bool, int> ResourceServer::DeleteSearchSingle(const char* search_key, std::map<const char*, int>* resourceMap){
	auto itr = (*resourceMap).find(const_cast<char*>(search_key));
	if (itr != (*resourceMap).end()) {
		auto value = itr->second;
		(*resourceMap).erase(itr);
		return std::make_pair(true, value);
	}
	return std::make_pair(false, itr->second);
};
std::pair<bool, ResourceServer::Mult> ResourceServer::DeleteSearchMult(const char* search_key, std::map<const char*, ResourceServer::Mult>* resourceMap) {
	auto itr = (*resourceMap).find(const_cast<char*>(search_key));
	if (itr != (*resourceMap).end()) {
		auto value = itr->second;
		(*resourceMap).erase(itr);
		return std::make_pair(true, value);
	}
	return std::make_pair(false, itr->second);
};

bool ResourceServer::Delete(const char* key, TYPE resouceType) {
	if (resouceType == TYPE::Handle) {
		auto handleValue = ResourceServer::DeleteSearchSingle(key, &_handleMap);
		if (handleValue.first) {
			DeleteGraph(handleValue.second);
			return true;
		}
	}
	else if (resouceType == TYPE::Mult) {
		auto multValue = ResourceServer::DeleteSearchMult(key, &_multMap);
		if (multValue.first) {
			for (int i = 0; i < multValue.second.AllNum; i++) {
				DeleteGraph(multValue.second.handle[i]);
			}
			return true;
		}	
	}
	else if (resouceType == TYPE::Efk) {
		auto effekseerValue = ResourceServer::DeleteSearchSingle(key, &_effekseerMap);
		if (effekseerValue.first) {
			DeleteEffekseerEffect(effekseerValue.second);
			return true;
		}
	}
	else if (resouceType == TYPE::Sound) {
		auto soundValue = ResourceServer::DeleteSearchSingle(key, &_soundMap);
		if (soundValue.first) {
			DeleteSoundMem(soundValue.second);
			return true;
		}
	}
	return false;
};

void ResourceServer::DeleteResourceAll() {
	//画像の削除
	for (auto itr = _handleMap.begin(); itr != _handleMap.end(); itr++) {
		DeleteGraph((*itr).second);
	}
	//エフェクシアのエフェクトの削除
	for (auto itr = _effekseerMap.begin(); itr != _effekseerMap.end(); itr++) {
		DeleteEffekseerEffect((*itr).second);
	}
	//モデルの削除
	for (auto itr = _modelMap.begin(); itr != _modelMap.end(); itr++) {
		for (int i = 0; i < itr->second.size(); i++) {
			MV1DeleteModel(itr->second.at(i));
		}
	}

	InitSoundMem();//音だけは読み込んだものをまとめて消せるのでこの関数で削除する
	//配列の削除
	_handleMap.clear();
	_effekseerMap.clear();
	_modelMap.clear();
	_soundMap.clear();
};
