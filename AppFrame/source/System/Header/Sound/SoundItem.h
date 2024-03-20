#pragma once

#include "dxlib.h"
#include "../../Header/Function/Easing.h"
#include <string>

class SoundServer;

class SoundItemBase
{
public:
	static const char FADE_IN;
	static const char FADE_OUT;
	enum class TYPE {
		BGM = 0,	// ストリーミング。ループ再生
		SE,			// １回のみ再生
		VOICE,		// ストリーミング。１回のみ再生
		ONESHOT,	// ストリーミング。１回のみ再生。即時再生し、停止したらサーバから削除
	};
	SoundItemBase(std::string filename);
	virtual ~SoundItemBase();
	void SetSoundServer(SoundServer* sndServer) { _sndServer = sndServer; }

	virtual void	Unload();
	virtual bool	IsLoad();
	virtual void	Play() = 0;
	virtual bool	IsPlay();
	virtual void	Stop();

	virtual int		GetVolume();
	virtual void	SetVolume() = 0;
	virtual int		GetPan();
	virtual void	SetPan(int pan);
	virtual int		GetFrequency();
	virtual void	SetFrequency(int frequency);
	virtual void	ResetFrequency();
	virtual void   SetFadeVolume();

	virtual TYPE	GetType() = 0;
	virtual char GetFade() { return IsFade; }
	virtual void SetFade(int param, int time) { IsFade = param; _fadeTime = time; _currentTime = GetNowCount(); }
	virtual std::string GetFileName() { return _filename; }
	virtual int		GetSoundHandle() { return _snd; }

	virtual void	Update() {}

protected:
	virtual void	PlayMem(int flg);

	int		_snd;
	std::string	_filename;
	int		_volume;
	int		_pan;
	int		_frequency;
	SoundServer* _sndServer;

	char IsFade; // 0 : none 1 : in 2 : out
	int _currentTime;
	int _fadeTime;
}; 

class SoundItemStatic : public SoundItemBase
{
	typedef	SoundItemBase	base;
public:
	SoundItemStatic(std::string filename) : base(filename) {
		SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
		_snd = LoadSoundMem(_filename.c_str());
		if (_frequency == 0) {
			_frequency = GetFrequencySoundMem(_snd);
		}
	}
	virtual ~SoundItemStatic() {}
};

class SoundItemSE : public SoundItemStatic
{
	typedef	SoundItemStatic	base;
public:
	SoundItemSE(std::string filename) : base(filename) {
	}
	virtual TYPE	GetType() { return TYPE::SE; }
	virtual void SetVolume()override;
	virtual void	Play() {
		Stop();		// SEは一度止めてから
		PlayMem(DX_PLAYTYPE_BACK);
	}
};

class SoundItemStream : public SoundItemBase
{
	typedef	SoundItemBase	base;
public:
	SoundItemStream(std::string filename) : base(filename) {
	}
	virtual ~SoundItemStream() {}
	virtual bool	IsLoad() { return true; }	// ストリーム再生のものは、ロードされているものとする
protected:
	void	StreamLoad() {
		if (_snd == -1) {
			SetCreateSoundDataType(DX_SOUNDDATATYPE_FILE);
			_snd = LoadSoundMem(_filename.c_str());
			if (_frequency == 0) {
				_frequency = GetFrequencySoundMem(_snd);
			}
		}
	}
	virtual void	Update()override {
		if (_snd != -1) {
			SetFadeVolume();
			if (IsPlay() == false) {
				Unload();		// ストリーム再生のものは、再生していなかったらメモリからアンロードする
			}
		}
	}
};

class SoundItemBGM : public SoundItemStream
{
	typedef	SoundItemStream	base;
public:
	SoundItemBGM(std::string filename) : base(filename) {
	}
	virtual TYPE	GetType() { return TYPE::BGM; }
	virtual void SetVolume()override;
	virtual void	Play();
};

class SoundItemVOICE : public SoundItemStream
{
	typedef	SoundItemStream	base;
public:
	SoundItemVOICE(std::string filename) : base(filename) {
	}
	virtual TYPE	GetType() { return TYPE::VOICE; }
	virtual void SetVolume()override;
	virtual void	Play() {
		StreamLoad();	// ストリーム再生のものをロードする
		if (IsPlay() == false) {	// VOICEは再生されていない場合のみ
			PlayMem(DX_PLAYTYPE_BACK);
		}
	}
};

class SoundItemOneShot : public SoundItemStream
{
	typedef	SoundItemStream	base;
public:
	SoundItemOneShot(std::string filename) : base(filename) {
	}
	virtual TYPE	GetType() { return TYPE::ONESHOT; }
	virtual void SetVolume()override;
	virtual void	Play() {
		StreamLoad();	// ストリーム再生のものをロードする
		PlayMem(DX_PLAYTYPE_BACK);
	}
	virtual void	Update();

};
