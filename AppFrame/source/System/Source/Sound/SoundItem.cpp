#include "../../Header/Sound/SoundItem.h"
#include "../../Header/Sound/SoundServer.h"

const char SoundItemBase::FADE_IN = 1;
const char SoundItemBase::FADE_OUT = 2;

SoundItemBase::SoundItemBase(std::string filename) {
	_filename = filename;
	_snd = -1;
	_volume = 0;
	_pan = 0;
	_frequency = 0;		// ファイルによって標準が違うので
	_sndServer = NULL;
	_currentTime = 0;
	_fadeTime = 0;
	IsFade = 0;
}
SoundItemBase::~SoundItemBase() {
	Unload();
}

void	SoundItemBase::Unload() {
	if (_snd != -1) {
		DeleteSoundMem(_snd);
		_snd = -1;
	}
}

bool	SoundItemBase::IsLoad() {
	// 非同期ロードが終わっているか？
	if (_snd != -1 && CheckHandleASyncLoad(_snd) == FALSE) {
		return true;
	}
	return false;
}

bool	SoundItemBase::IsPlay() {
	if (_snd != -1 && CheckSoundMem(_snd) == 1) {
		return true;
	}
	return false;
}

void	SoundItemBase::Stop() {
	if (_snd != -1) {
		StopSoundMem(_snd);
	}
}

int		SoundItemBase::GetVolume() { return _volume; }
int		SoundItemBase::GetPan() { return _pan; }
void	SoundItemBase::SetPan(int pan) {
	_pan = pan;
	if (_snd != -1) {
		ChangePanSoundMem(_pan, _snd);
	}
}

int		SoundItemBase::GetFrequency() { return _frequency; }
void	SoundItemBase::SetFrequency(int frequency) {
	_frequency = frequency;
	if (_snd != -1 && _frequency != 0) {
		SetFrequencySoundMem(_frequency, _snd);
	}
}
void	SoundItemBase::ResetFrequency() {
	_frequency = 0;
	if (_snd != -1) {
		ResetFrequencySoundMem(_snd);
		_frequency = GetFrequencySoundMem(_snd);
	}
}

void SoundItemBase::SetFadeVolume() {
	if (IsFade == 1) {
	// FadeIn
		int nowTime = GetNowCount() - _currentTime;
		_volume = Easing::Linear(nowTime, 0, _sndServer->GetBgmVolume(), _fadeTime);
		if (nowTime >= _fadeTime) {
			IsFade = 0;
		}
	}
	else if (IsFade == 2) {
	// FadeOut
		int nowTime = GetNowCount() - _currentTime;
		_volume = Easing::Linear(nowTime, _sndServer->GetBgmVolume(), 0, _fadeTime);
		if (nowTime >= _fadeTime) {
			IsFade = 0;
		}
	}
	
	if (_snd != -1) {
		ChangeVolumeSoundMem(_volume, _snd);
	}
};

void	SoundItemBase::PlayMem(int flg) {
	// 再生前に音量等を設定
	SetVolume();
	SetPan(_pan);
	SetFrequency(_frequency);
	PlaySoundMem(_snd, flg, TRUE);
}

void	SoundItemSE::SetVolume() {
	_volume = _sndServer->GetSeVolume();
	if (_snd != -1) {
		ChangeVolumeSoundMem(_volume, _snd);
	}
}

void	SoundItemBGM::SetVolume() {
	_volume = _sndServer->GetBgmVolume();
	if (_snd != -1) {
		ChangeVolumeSoundMem(_volume, _snd);
	}
}

void	SoundItemBGM::Play() {
	if (_sndServer) {
		_sndServer->StopType(TYPE::BGM);
	}
	StreamLoad();	// ストリーム再生のものをロードする
	if (IsPlay() == false) {	// BGMは再生されていない場合のみ
		PlayMem(DX_PLAYTYPE_LOOP);
	}
}

void	SoundItemVOICE::SetVolume() {
	_volume = _sndServer->GetVoiceVolume();
	if (_snd != -1) {
		ChangeVolumeSoundMem(_volume, _snd);
	}
}

void	SoundItemOneShot::Update() {
	int snd = _snd;
	base::Update();
	if (snd != -1 && _snd == -1) {
		// 再生されていたがUnload()されている場合、自分を削除
		_sndServer->Del(this);
	}
}

void	SoundItemOneShot::SetVolume() {
	_volume = _sndServer->GetSeVolume();
	if (_snd != -1) {
		ChangeVolumeSoundMem(_volume, _snd);
	}
}
