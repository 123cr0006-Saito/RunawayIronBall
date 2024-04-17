#include "../../Header/Sound/SoundItem.h"
#include "../../Header/Sound/SoundServer.h"

SoundServer::SoundServer() {
	_isUpdate = false;
	_cntOneShot = 0;
	_SeVolume = _VoiceVolume = 255;
	_BgmVolume = 150;
}
SoundServer::~SoundServer() {
	Clear();
}

void SoundServer::Clear() {
	for (auto&& e : _v) {
		delete e.second;
	}
	for (auto&& e : _vAdd) {
		delete e.second;
	}
	for (auto&& e : _vDel) {
		delete e.second;
	}
	_v.clear();
	_vAdd.clear();
	_vDel.clear();
}

void SoundServer::Add(SoundItemOneShot* snd) {
	std::string name = "__ONESHOT_SOUND_" + std::to_string(_cntOneShot);
	_cntOneShot++;
	Add(name, snd);
	snd->Play();		// OneShot�͓o�^�����u�ԍĐ�����
}

void SoundServer::Add(std::string name, SoundItemOneShot* snd) {
	Add(name, snd);
	snd->Play();		// OneShot�͓o�^�����u�ԍĐ�����
}

void SoundServer::Add(std::string name, SoundItemBase* snd) {
	if (_isUpdate) {
		_vAdd[name] = snd;
	}
	else {
		_v[name] = snd;
	}
	snd->SetSoundServer(this);
}

bool SoundServer::Del(SoundItemBase* snd) {
	for (auto&& e : _v) {
		if (e.second == snd) {
			if (_isUpdate) {
				_vDel[e.first] = e.second;
			}
			else {
				std::string name = e.first;
				delete _v[name];
				_v.erase(name);
			}
			return true;
		}
	}
	return false;
}

bool SoundServer::Del(std::string name) {
	if (_v.count(name) > 0) {
		return Del(_v[name]);
	}
	return false;
}

SoundItemBase* SoundServer::Get(std::string name) {
	if (_v.count(name) > 0) { return _v[name]; }
	return NULL;
}

SoundItemBase* SoundServer::Search(std::string name) {
	if (GetASyncLoadNum() == 0) {
		auto itr = _v.find(name);
		if (itr != _v.end()) {
			return itr->second;
		}
#ifdef _DEBUG
		else {
			//�G���[
			//�t�@�C�������� ���� �ǂݍ��܂�Ă��܂���
			std::string errar = "�t�@�C�������� ���� �ǂݍ��܂�Ă��܂���";
			MessageBox(NULL, errar.c_str(), "�G���[", MB_OK);
		}
#endif
	}
#ifdef _DEBUG
	else {
		//�G���[
		//�񓯊��ǂݍ��ݒ��ł��B
		std::string errar = "�񓯊��ǂݍ��ݒ��ł��B";
		MessageBox(NULL, errar.c_str(), "�G���[", MB_OK);
	}
#endif
	return nullptr;
};

bool SoundServer::DirectPlay(std::string name) {
	auto playSound = Search(name);
	if (playSound) {
		playSound->Play();
		return true;
	}
	return false;
};

bool SoundServer::RandomPlay(std::vector<std::string> name) {
	int random = rand() % name.size();
	if (DirectPlay(name.at(random))) {
		return true;
	}
	return false;
};

std::vector<SoundItemBase*> SoundServer::NowPlayingSearch() {
	std::vector<SoundItemBase*> nowPlaying;
	for (auto&& e : _v) {
		if (e.second->IsPlay()) {
			nowPlaying.push_back(e.second);
		}
	}
	return nowPlaying;
};

std::vector<SoundItemBase*> SoundServer::NowPlayingSearchType(SoundItemBase::TYPE type) {
	std::vector<SoundItemBase*> nowPlaying;
	for (auto&& e : _v) {
		if (e.second->GetType() == type && e.second->IsPlay()) {
			nowPlaying.push_back(e.second);
		}
	}
	return nowPlaying;
};

void SoundServer::StopAll() {
	auto stopSound = NowPlayingSearch();
	for (auto&& e : stopSound) {
		e->Stop();
	}
};

void SoundServer::StopType(SoundItemBase::TYPE type) {
	auto stopSound = NowPlayingSearchType(type);
	for (auto&& e : stopSound) {
		if (e->GetFade() != SoundItemBase::FADE_OUT) {
			e->Stop();
		}
	}
}

void SoundServer::BgmFadeOut(int time) {
	auto fadeSound = NowPlayingSearchType(SoundItemBase::TYPE::BGM);
	for (auto&& e : fadeSound) {
		e->SetFade(SoundItemBase::FADE_OUT,time);
	}
};

void SoundServer::BgmFadeIn(std::string name,int time){
	auto sound = Search(name);
	//�����������炱���ŃT�E���h�̉��ʂ�0�ɂ��Ȃ��Ƃ����Ȃ����� 1F����邩��
	sound->SetFade(SoundItemBase::FADE_IN,time); 
	sound->Play();
};

void SoundServer::Update() {
	// �ǉ����X�g�̂��̂�ǉ�
	for (auto&& e : _vAdd) { Add(e.first, e.second); }
	_vAdd.clear();

	// Update����
	_isUpdate = true;
	for (auto&& e : _v) {
		e.second->Update();
	}
	_isUpdate = false;

	// �폜���X�g�̂��̂��폜
	for (auto&& e : _vDel) { Del(e.second); }
	_vDel.clear();
}


void SoundServer::SetBgmVolume(int volume) { 
	_BgmVolume = volume; 
	auto bgmList = NowPlayingSearchType(SoundItemBase::TYPE::BGM);
	for(auto&& bgm : bgmList){
	   bgm->SetVolume();
	}
}