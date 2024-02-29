#include "EffekseerBase.h"

//エフェクシアの関数でUpdateEffekseer3Dと描画関係の関数は
//1Ｆに１回だけ呼び出す
//何回も呼び出すとそのぶん一緒に処理される

EffekseerBase::EffekseerBase(std::string name, VECTOR* pos,float size,float speed,bool loopFlag) :
	EffectBase(),
	_effectResourceHandle(ResourceServer::LoadEffekseerEffect(_T(name.c_str()),_T(name.c_str()))),
	_pos(*pos),
	_speed(speed),
	_size(size),
	_loopFlag(loopFlag)
{
	_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
	SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
	SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);
};
EffekseerBase::EffekseerBase(int handle, VECTOR* pos, float size, float speed , bool loopFlag ) :
	EffectBase(),
	_effectResourceHandle(handle),
	_pos(*pos),
	_speed(speed),
	_size(size),
	_loopFlag(loopFlag)
{
	_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
	SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
	SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);
};

EffekseerBase::~EffekseerBase() {
	
};

bool EffekseerBase::Process() {

	SetPosPlayingEffekseer3DEffect(_playingEffectHandle, _pos.x, _pos.y, _pos.z);

	if (IsEffekseer3DEffectPlaying(_playingEffectHandle) == -1) {
		if (_loopFlag) {
			_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
			SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
			SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);
		}
		else {
			_IsPlay = false;
		}
	}

	return true;
};

bool EffekseerBase::Render() {
	PlayEffekseer3DEffect(_playingEffectHandle);
	return true;
};