#include "EffekseerPosSynchro.h"

EffekseerPosSynchro::EffekseerPosSynchro(std::string name, VECTOR* pos, float size, float speed, bool loopFlag) :
	base( name, pos, size, speed ,loopFlag),
	_pos(pos)
{
};

EffekseerPosSynchro::EffekseerPosSynchro(int handle, VECTOR* pos, float size, float speed, bool loopFlag):base(handle, pos, size, speed, loopFlag), 
_pos(pos)
{

};

EffekseerPosSynchro::~EffekseerPosSynchro() {
	base::~EffekseerBase();
	_pos = nullptr;
};

bool EffekseerPosSynchro::Process() {

	SetPosPlayingEffekseer3DEffect(_playingEffectHandle, (*_pos).x, (*_pos).y, (*_pos).z);

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

bool EffekseerPosSynchro::Render() {
	PlayEffekseer3DEffect(_playingEffectHandle);
	return true;
};