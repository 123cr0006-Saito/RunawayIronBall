#define _CRT_SECURE_NO_WARNINGS 1
#include "../../Header/Resource/ResourceServer.h"

std::map<const char*, int> ResourceServer::handle_server;
std::map<const char*, int> ResourceServer::sound_server;
std::map<const char*, ResourceServer::Div> ResourceServer::div_server;
std::map<const char*, ResourceServer::Div> ResourceServer::mult_server;
std::map<const char*, int >ResourceServer::effekseer_server;
std::map<const char*, std::vector<int> >ResourceServer::model_server;


int ResourceServer::LoadGraph(const char* key_name, const char* handle_name) {

	int value = 0;

	auto itr = handle_server.find(key_name);
	if (itr != handle_server.end()) {
		//記録されたものが見つかったので値を返す
		value = itr->second;
	}
	else {
		//記録された名前がなかったので読み込み
		value = ::LoadGraph(handle_name);
		handle_server[key_name] = value;
	}

	return value;
};

int ResourceServer::LoadSound(const char* key_name, const char* sound_name) {

	int value = 0;

	auto itr = sound_server.find(key_name);
	if (itr != sound_server.end()) {
		//記録されたものが見つかったので値を返す
		value = itr->second;
	}
	else {
		//記録された名前がなかったので読み込み
		value = ::LoadSoundMem(sound_name);
		sound_server[key_name] = value;
	}

	return value;
};

int ResourceServer::LoadDivGraph(const char* key_name, const char* handle_name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf) {

	int value = 0;
	auto itr = div_server.find(key_name);

	if (itr != div_server.end()) {
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
			div_server[key_name].AllNum = AllNum;
			div_server[key_name].handle = buf;
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
	auto itr = mult_server.find(key_name);

	if (itr != mult_server.end()) {
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
		mult_server[key_name].AllNum = AllNum;
		mult_server[key_name].handle = buf;
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

	auto itr = effekseer_server.find(key_name);
	if (itr != effekseer_server.end()) {
		//記録されたものが見つかったので値を返す
		value = itr->second;
	}
	else {
		//記録された名前がなかったので読み込み
		value = ::LoadEffekseerEffect(handle_name);
		handle_server[key_name] = value;
	}

	return value;
};

int ResourceServer::MV1LoadModel(const char* key_name, const char* model_name) {
	int value = 0;

	auto itr = model_server.find(key_name);
	if (itr != model_server.end()) {
		//記録されたものが見つかったのでオリジナルをコピーして返す
		value = ::MV1DuplicateModel(itr->second.at(0));
		//後で削除できるように番号も持っておく
		model_server[key_name].push_back(value);
	}
	else {
		//記録された名前がなかったので読み込み
		value = ::MV1LoadModel(model_name);
		model_server[key_name].push_back(value);
	}

	return value;
};

int ResourceServer::SearchGraph(const char* serch_name) {
	//画像の検索　あった場合は画像の値を返す
	auto itr = handle_server.find(serch_name);
	if (itr != handle_server.end()) {
		return itr->second;
	}
	return false;
};

template<typename T >
std::pair<bool,T> ResourceServer::Search(const char* search_key, std::map<const char*, T>* resourceMap) {
	//auto itr = (*resourceMap).find(search_key);
	auto itr = std::find((*resourceMap).begin(), (*resourceMap).end(), searc_key);
	if (itr != resourceMap.end()) {
		T value = itr->second;
		resourceMap->erase(itr);
		return std::make_pair(true,value);
	}
	return std::make_pair(false, itr->second);
};

bool ResourceServer::Delete(const char* key, TYPE resouceType) {
	/*switch (resouceType) {
	case TYPE::Handle:
		auto handleValue = ResourceServer::Search<int>(key, &handle_server);
		if (handleValue.first) {
			DeleteGraph(handleValue.second);
			return true;
		}
		break;
	case TYPE::Div:
		auto divValue = ResourceServer::Search<Div>(key, &div_server);
		if (divValue.first) {
			for (int i = 0; i < divValue.second.AllNum; i++) {
				DeleteGraph(divValue.second.handle[i]);
				return true;
			}
		}
		break;
	case TYPE::Mult:
		auto multValue = ResourceServer::Search<Div>(key, &mult_server);
		if (multValue.first) {
			for (int i = 0; i < multValue.second.AllNum; i++) {
				DeleteGraph(multValue.second.handle[i]);
				return true;
			}
		}
		break;
	case TYPE::Efk:
		auto effekseerValue = ResourceServer::Search<int>(key, &effekseer_server);
		if (effekseerValue.first) {
			DeleteEffekseerEffect(effekseerValue.second);
			return true;
		}
		break;
	case TYPE::Sound:
		auto soundValue = ResourceServer::Search<int>(key, &sound_server);
		if (soundValue.first) {
			DeleteSoundMem(soundValue.second);
			return true;
		}
		break;
	}*/

	if (resouceType == TYPE::Handle) {
		auto handleValue = ResourceServer::Search<int>(key, &handle_server);
		if (handleValue.first) {
			DeleteGraph(handleValue.second);
			return true;
		}
	}
	else if (resouceType == TYPE::Div) {
		auto divValue = ResourceServer::Search<Div>(key, &div_server);
		if (divValue.first) {
			for (int i = 0; i < divValue.second.AllNum; i++) {
				DeleteGraph(divValue.second.handle[i]);
				return true;
			}
		}
	}
	else if (resouceType == TYPE::Mult) {
		auto multValue = ResourceServer::Search<Div>(key, &mult_server);
		if (multValue.first) {
			for (int i = 0; i < multValue.second.AllNum; i++) {
				DeleteGraph(multValue.second.handle[i]);
				return true;
			}
		}
	}
	else if (resouceType == TYPE::Efk) {
		auto effekseerValue = ResourceServer::Search<int>(key, &effekseer_server);
		if (effekseerValue.first) {
			DeleteEffekseerEffect(effekseerValue.second);
			return true;
		}
	}
	else if (resouceType == TYPE::Sound) {
		auto soundValue = ResourceServer::Search<int>(key, &sound_server);
		if (soundValue.first) {
			DeleteSoundMem(soundValue.second);
			return true;
		}
	}
	return false;
};

void ResourceServer::DeleteResourceAll() {
	//画像の削除
	for (auto itr = handle_server.begin(); itr != handle_server.end(); itr++) {
		DeleteGraph((*itr).second);
	}
	//構造体の画像の削除
	for (auto itr = div_server.begin(); itr != div_server.end(); itr++) {
		for (int i = 0; i < (*itr).second.AllNum; i++) {
			DeleteGraph((*itr).second.handle[i]);
		}
	}
	//エフェクシアのエフェクトの削除
	for (auto itr = effekseer_server.begin(); itr != effekseer_server.end(); itr++) {
		DeleteEffekseerEffect((*itr).second);
	}
	//モデルの削除
	for (auto itr = model_server.begin(); itr != model_server.end(); itr++) {
		for (int i = 0; i < itr->second.size(); i++) {
			MV1DeleteModel(itr->second.at(i));
		}
	}

	InitSoundMem();//音だけは読み込んだものをまとめて消せるのでこの関数で削除する
	//配列の削除
	handle_server.clear();
	div_server.clear();
	effekseer_server.clear();
	model_server.clear();
	sound_server.clear();
};
