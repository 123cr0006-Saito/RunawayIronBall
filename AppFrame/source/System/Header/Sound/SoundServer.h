#pragma once

#include "SoundItem.h"

#include <vector>
#include <string>
#include <map>

class SoundServer
{
public:
	SoundServer();
	virtual ~SoundServer();

	void Clear();
	void Add(SoundItemOneShot* snd);
	void Add(std::string name, SoundItemOneShot* snd);
	void Add(std::string name, SoundItemBase* snd);
	bool Del(std::string name);
	bool Del(SoundItemBase *snd);
	SoundItemBase* Get(std::string name);

	std::map<std::string, SoundItemBase*>& GetVector() { return _v; }

	SoundItemBase* Search(std::string name);
	std::vector<SoundItemBase*> NowPlayingSearch();
	std::vector<SoundItemBase*> NowPlayingSearchType(SoundItemBase::TYPE type);

	bool DirectPlay(std::string name);
	bool RandomPlay(std::vector<std::string> name);

	void StopAll();
	void StopType(SoundItemBase::TYPE type);
	void BgmFadeOut(int time);
	void BgmFadeIn(std::string name,int time);

	void Update();

	void SetSeVolume(int volume) {_SeVolume = volume;}
	void SetBgmVolume(int volume);
	void SetVoiceVolume(int volume) { _VoiceVolume = volume; }

	int GetSeVolume() { return _SeVolume ; }
	int GetBgmVolume() { return  _BgmVolume ; }
	int GetVoiceVolume() { return _VoiceVolume ; }

protected:
	bool	_isUpdate;
	std::map<std::string, SoundItemBase*> _v;

	std::map<std::string, SoundItemBase*> _vAdd;
	std::map<std::string, SoundItemBase*> _vDel;

	int		_cntOneShot;

	int _SeVolume, _BgmVolume, _VoiceVolume;
};

