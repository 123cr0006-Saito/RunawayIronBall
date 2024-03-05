#include "EffekseerRotation.h"
EffekseerRotation::EffekseerRotation(std::string name, VECTOR* pos, float size, VECTOR rotation,float speed, bool loopFlag) :
	EffekseerBase(name, pos, size),
    _pos(pos)
{
	SetRotationPlayingEffekseer3DEffect(_playingEffectHandle, rotation.x, rotation.y, rotation.z);
};

EffekseerRotation::EffekseerRotation(int handle, VECTOR* pos, float size, VECTOR rotation, float speed, bool loopFlag) :
	EffekseerBase(handle,pos,size),
	_pos(pos)
{
	SetRotationPlayingEffekseer3DEffect(_playingEffectHandle, 0, rotation.y, 0);
};

EffekseerRotation::~EffekseerRotation() {
	base::~EffekseerBase();
	_pos = nullptr;
};

bool EffekseerRotation::Process(){
	SetPosPlayingEffekseer3DEffect(_playingEffectHandle, (*_pos).x, (*_pos).y+50.0f, (*_pos).z);

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

bool EffekseerRotation::Render() {
	PlayEffekseer3DEffect(_playingEffectHandle);
	return true;
};