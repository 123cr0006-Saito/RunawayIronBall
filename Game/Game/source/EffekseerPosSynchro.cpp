#include "EffekseerPosSynchro.h"

EffekseerPosSynchro::EffekseerPosSynchro(std::string name, VECTOR* pos, float size, VECTOR rotation, float height ,float speed, bool loopFlag) :
	base( name, pos, size, speed ,loopFlag),
	_pos(pos),
	_height(height)
{
	if (rotation.x != 0.0f) {
		VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
		rotation.y = 0.0f;
		float angle = Math::CalcVectorAngle(vBase, rotation);
		angle *= rotation.x < 0.0f ? 1.0f : -1.0f;
		_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
		SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
		SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);
		SetRotationPlayingEffekseer3DEffect(_playingEffectHandle, 0, angle, 0);
	}
};

EffekseerPosSynchro::EffekseerPosSynchro(int handle, VECTOR* pos, float size, VECTOR rotation, float height, float speed, bool loopFlag) :base(handle, pos, size, speed, loopFlag),
_pos(pos),
_height(height)
{
	if (rotation.x != 0.0f) {
		VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
		rotation.y = 0.0f;
		float angle = Math::CalcVectorAngle(vBase, rotation);
		angle *= rotation.x < 0.0f ? 1.0f : -1.0f;
		_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
		SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
		SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);
		SetRotationPlayingEffekseer3DEffect(_playingEffectHandle, 0, angle, 0);
	}
	_currentTime = GetNowCount();
};

EffekseerPosSynchro::~EffekseerPosSynchro() {
	//delete _pos;  _pos = nullptr;
};

bool EffekseerPosSynchro::Process() {
	float endTime = 50.0f / 60.0f * 1000;
	SetPosPlayingEffekseer3DEffect(_playingEffectHandle, (*_pos).x, (*_pos).y + _height, (*_pos).z);

	if (GetNowCount() - _currentTime >= endTime) {
		if (_loopFlag) {
			_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
			SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
			SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);
		}
		else {
			StopEffekseer3DEffect(_playingEffectHandle);
			_IsPlay = false;
		}
	}

	return true;
};

bool EffekseerPosSynchro::Render() {
	return true;
};