//#include "EffekseerRotation.h"
//EffekseerRotation::EffekseerRotation(std::string name, VECTOR* pos, float size, VECTOR* rotation,float speed, bool loopFlag) :
//	EffekseerBase(name, pos, size),
//    _pos(pos),
//	_rotation(rotation)
//{
//	//VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
//	//rotation.y = 0.0f;
//	//float angle = Math::CalcVectorAngle(vBase, rotation);
//	//angle *= rotation.x < 0.0f ? 1.0f : -1.0f;
//	//SetRotationPlayingEffekseer3DEffect(_playingEffectHandle, rotation.x, angle, rotation.z);
//};
//
//EffekseerRotation::EffekseerRotation(int handle, VECTOR* pos, float size, VECTOR* rotation, float speed, bool loopFlag) :
//	EffekseerBase(handle,pos,size),
//	_pos(pos),
//	_rotation(rotation)
//{
//	VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
//	VECTOR Rotation = *(_rotation);
//	Rotation.y = 0.0f;
//	float angle = Math::CalcVectorAngle(vBase, Rotation);
//	angle *= Rotation.x < 0.0f ? 1.0f : -1.0f;
//	SetRotationPlayingEffekseer3DEffect(_playingEffectHandle, 0, angle, 0);
//};
//
//EffekseerRotation::~EffekseerRotation() {
//	base::~EffekseerBase();
//	_pos = nullptr;
//};
//
//bool EffekseerRotation::Process(){
//	SetPosPlayingEffekseer3DEffect(_playingEffectHandle, (*_pos).x, (*_pos).y+50, (*_pos).z);
//
//	//VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
//	//VECTOR rotation = *(_rotation);
//	//rotation.y = 0.0f;
//	//float angle = Math::CalcVectorAngle(vBase, rotation);
//	//angle *= rotation.x < 0.0f ? 1.0f : -1.0f;
//	//SetRotationPlayingEffekseer3DEffect(_playingEffectHandle, 0, angle, 0);
//
//	if (IsEffekseer3DEffectPlaying(_playingEffectHandle) == -1) {
//		if (_loopFlag) {
//			_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
//			SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
//			SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);
//		}
//		else {
//			_IsPlay = false;
//		}
//	}
//
//	return true;
//};
//
//bool EffekseerRotation::Render() {
//		DrawEffekseer3D_Draw(_playingEffectHandle);
//	return true;
//};

//------------------------------------------------------------------------------------------------------------------------------------------

#include "EffekseerRotation.h"
EffekseerRotation::EffekseerRotation(std::string name, VECTOR* pos, float size, VECTOR* rotation, float speed, bool loopFlag) :
	EffekseerBase(name, pos, size),
	_pos(pos),
	_rotation(rotation)
{
	//VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
	//rotation.y = 0.0f;
	//float angle = Math::CalcVectorAngle(vBase, rotation);
	//angle *= rotation.x < 0.0f ? 1.0f : -1.0f;
	//SetRotationPlayingEffekseer3DEffect(_playingEffectHandle, rotation.x, angle, rotation.z);
};

EffekseerRotation::EffekseerRotation(int handle, VECTOR* pos, float size, VECTOR* rotation, float speed, bool loopFlag) :
	EffekseerBase(handle, pos, size),
	_pos(pos),
	_rotation(rotation)
{
	VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
	VECTOR Rotation = *(_rotation);
	Rotation.y = 0.0f;
	float angle = Math::CalcVectorAngle(vBase, Rotation);
	angle *= Rotation.x < 0.0f ? 1.0f : -1.0f;
	for (int i = 0; i < 5; i++) {
		_playingEffectHandle[i] = PlayEffekseer3DEffect(_effectResourceHandle);
		SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle[i], _speed);
		SetScalePlayingEffekseer3DEffect(_playingEffectHandle[i], _size, _size, _size);
		SetRotationPlayingEffekseer3DEffect(_playingEffectHandle[i], 0, angle + 50.0f * 3.141592 / 180, 0);
	}
};

EffekseerRotation::~EffekseerRotation() {
	base::~EffekseerBase();
	_pos = nullptr;
};

bool EffekseerRotation::Process() {

	VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
	VECTOR Rotation = *(_rotation);
	Rotation.y = 0.0f;
	float angle = Math::CalcVectorAngle(vBase, Rotation);
	angle *= Rotation.x < 0.0f ? 1.0f : -1.0f;

	for (int i = 0; i < 6; i++) {
		SetPosPlayingEffekseer3DEffect(_playingEffectHandle[i], (*_pos).x, (*_pos).y + 50, (*_pos).z);

			SetRotationPlayingEffekseer3DEffect(_playingEffectHandle[i], 0, angle + 360.0f / 5 * i * 3.141592 / 180, 0);

		if (IsEffekseer3DEffectPlaying(_playingEffectHandle[i]) == -1) {
			if (_loopFlag) {
				_playingEffectHandle[i] = PlayEffekseer3DEffect(_effectResourceHandle);
				SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle[i], _speed);
				SetScalePlayingEffekseer3DEffect(_playingEffectHandle[i], _size, _size, _size);
			}
			else {
				_IsPlay = false;
			}
		}
	}

	return true;
};

bool EffekseerRotation::Render() {
	//DrawEffekseer3D_Draw(_playingEffectHandle[i]);
	return true;
};