#define _CRT_SECURE_NO_WARNINGS 1
#include "../../Header/Resource/ResourceServer.h"

std::unordered_map<std::string, int> ResourceServer::_handleMap;
std::unordered_map<std::string, int> ResourceServer::_soundMap;
std::unordered_map<std::string, ResourceServer::Mult> ResourceServer::_multMap;
std::unordered_map<std::string, int >ResourceServer::_effekseerMap;
std::unordered_map<std::string, std::vector<int> >ResourceServer::_modelMap;
std::unordered_map<std::string, int >ResourceServer::_modelOriginMap;

int ResourceServer::Load(std::string key, std::string handleName) {
	int value = 0;
	int size = handleName.length();
	std::string extension = handleName.substr(size - 3); //3文字分の拡張子を取得
	if (extension == "png" || extension == "jpg" || extension == "peg" || extension == "mp4") {
		value = LoadGraph(key, handleName);
	}
	else if (extension == "efk" || extension == "efc") {
		value = LoadEffekseerEffect(key, handleName);
	}
	else if (extension == "mv1") {
		value = MV1LoadModel(key, handleName,false);
	}
	return value;
};

int ResourceServer::LoadGraph(std::string key_name, std::string handle_name) {

	int value = 0;

	auto itr = _handleMap.find(key_name);
	if (itr != _handleMap.end()) {
		//記録されたものが見つかったので値を返す
		value = itr->second;
	}
	else {
		//記録された名前がなかったので読み込み
		value = ::LoadGraph(handle_name.c_str());
		_handleMap[key_name] = value;
	}

	return value;
};

int ResourceServer::LoadSound(std::string key_name, std::string sound_name) {

	int value = 0;

	auto itr = _soundMap.find(key_name);
	if (itr != _soundMap.end()) {
		//記録されたものが見つかったので値を返す
		value = itr->second;
	}
	else {
		//記録された名前がなかったので読み込み
		value = ::LoadSoundMem(sound_name.c_str());
		_soundMap[key_name] = value;
	}

	return value;
};

int ResourceServer::LoadDivGraph(std::string key_name, std::string handle_name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf) {

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
		value = ::LoadDivGraph(handle_name.c_str(), AllNum, XNum, YNum, XSize, YSize, buf);

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

int ResourceServer::LoadDivGraph(std::string key_name, std::string handle_name, int AllNum, int XNum, int YNum, int XSize, int YSize) {
	// 返り値 -1 : 失敗 1 : 成功 2 : すでに値があります

	int value = 0;
	auto itr = _multMap.find(key_name);

	if (itr != _multMap.end()) {
		return 2;
	}
	else {
		//記録された名前がなかったので読み込み
		//読み込む枚数がわからないためメモリを動的確保
		int* buf = new int[AllNum];
		value = ::LoadDivGraph(handle_name.c_str(), AllNum, XNum, YNum, XSize, YSize, buf);

		if (value != -1) {
			//エラーではなかった場合
			//最大枚数と読み込みんだ枚数分値を確保
			_multMap[key_name].AllNum = AllNum;
			_multMap[key_name].handle = buf;
			//読み込んだ値を移動
			//std::swap(HandleBuf, buf);
			return 1;
		}
	}
	//返すのは成功だった場合の0 失敗だった場合の-1
	return -1;
};

int ResourceServer::LoadMultGraph(std::string key_name, std::string handle_name, std::string extension, int AllNum, int* HandleBuf) {

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


int ResourceServer::LoadMultGraph(std::string key_name, std::string handle_name, std::string extension, int AllNum) {

	auto itr = _multMap.find(key_name);

	if (itr != _multMap.end()) {
		return 2;
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
	}
	return 1;
};

int ResourceServer::LoadEffekseerEffect(std::string key_name, std::string handle_name) {

	int value = 0;

	auto itr = _effekseerMap.find(key_name);
	if (itr != _effekseerMap.end()) {
		//記録されたものが見つかったので値を返す
		value = itr->second;
	}
	else {
		//記録された名前がなかったので読み込み
		value = ::LoadEffekseerEffect(handle_name.c_str());
		_effekseerMap[key_name] = value;
	}

	return value;
};

int ResourceServer::MV1LoadModel(std::string key_name, std::string model_name, bool duplicate) {
	int value = 0;

	auto itr = _modelOriginMap.find(key_name + "_Origin");
	if (itr != _modelOriginMap.end()) {
		//記録されたものが見つかったのでオリジナルをコピーして返す
		value = MV1DuplicateModel(itr->second);
		//後で削除できるように番号も持っておく
		_modelMap[key_name].push_back(value);
	}
	else {
		//記録された名前がなかったので読み込み
		int originModel = ::MV1LoadModel(model_name.c_str());
		if (originModel != -1) {
			_modelOriginMap[key_name + "_Origin"] = originModel;
			// オリジナルから複製
			if (duplicate)
			{
				value = MV1DuplicateModel(originModel);
				_modelMap[key_name].push_back(value);
			}
		}
	}

	return value;
};

int ResourceServer::SearchSingle(std::string search_key, TYPE resouceType) {
	std::unordered_map<std::string, int>* resourceMap = nullptr;
	//リソースの種類によって検索するリソースを変更
	switch (resouceType) {
	case TYPE::Handle:
		resourceMap = &_handleMap;
		break;
	case TYPE::Efk:
		resourceMap = &_effekseerMap;
		break;
	case TYPE::Sound:
		resourceMap = &_soundMap;
		break;
	}

	auto itr = (*resourceMap).find(const_cast<char*>(search_key.c_str()));
	if (itr != (*resourceMap).end()) {
		return itr->second;
	}

	return -1;
};

bool ResourceServer::SearchMult(std::string search_key, int* handle, int size) {
	auto itr = _multMap.find(search_key);
	if (itr != _multMap.end()) {
		for (int i = 0; i < size; i++) {
			handle[i] = itr->second.handle[i];
		}
		return true;
	}
	return false;
};

ResourceServer::Mult ResourceServer::SearchMult(std::string search_key) {
	auto itr = _multMap.find(search_key);
	if (itr != _multMap.end()) {
		return itr->second;
	}
	Mult mult;
	return mult;
};

std::pair<bool, int> ResourceServer::DeleteSearchSingle(std::string search_key, std::unordered_map<std::string, int>* resourceMap){
	auto itr = (*resourceMap).find(search_key);
	if (itr != (*resourceMap).end()) {
		auto value = itr->second;
		(*resourceMap).erase(itr);
		return std::make_pair(true, value);
	}
	return std::make_pair(false, itr->second);
};

std::pair<bool, ResourceServer::Mult> ResourceServer::DeleteSearchMult(std::string search_key, std::unordered_map<std::string, ResourceServer::Mult>* resourceMap) {
	auto itr = (*resourceMap).find(search_key);
	if (itr != (*resourceMap).end()) {
		auto value = itr->second;
		(*resourceMap).erase(itr);
		return std::make_pair(true, value);
	}
	return std::make_pair(false, itr->second);
};

bool ResourceServer::Delete(std::string key, TYPE resouceType) {
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

bool ResourceServer::MV1DeleteModel(std::string key, int model) {
   auto itr = _modelMap.at(key);
   if (itr.size() > 0) {
	  for (int i = 0; i < itr.size(); i++) {
		 if (itr.at(i) == model) {
			::MV1DeleteModel(model);
			_modelMap.at(key).erase(itr.begin() + i);
			return true;
		 }
	  }
   }
	return false;
};

bool ResourceServer::MV1DeleteModelAll(std::string key) {
	auto itr = _modelMap.find(key);
	if(itr != _modelMap.end()){
		for (int i = 0; i < itr->second.size(); i++) {
			::MV1DeleteModel(itr->second.at(i));
		}
		_modelMap.at(key).clear();
		return true;
	}
	return false;
};

bool ResourceServer::MV1DeleteModelOrigin(std::string key) {
	std::string originKey = key + "_Origin";
	auto itr = _modelMap.find(originKey);
	if (itr != _modelMap.end()) {
		for (int i = 0; i < itr->second.size(); i++) {
			::MV1DeleteModel(itr->second.at(i));
		}
		_modelMap.at(originKey).clear();
		return true;
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
			::MV1DeleteModel(itr->second.at(i));
		}
	}

	for (auto itr = _multMap.begin(); itr != _multMap.end(); ++itr) {
		for (int i = 0; i < itr->second.AllNum; i++) {
			DeleteGraph(itr->second.handle[i]);
		}
		delete[] itr->second.handle;
	}

	InitSoundMem();//音だけは読み込んだものをまとめて消せるのでこの関数で削除する
	//配列の削除
	_handleMap.clear();
	_effekseerMap.clear();
	_modelMap.clear();
	_soundMap.clear();
};
