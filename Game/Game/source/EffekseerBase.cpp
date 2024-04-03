//----------------------------------------------------------------------
// @filename EffekseerBase.cpp
// ��date: 2024/02/8
// ��author: saito ko
// @explanation
// �G�t�F�N�V�A���Đ����邽�߂̊�{�I�ȃN���X
//----------------------------------------------------------------------
#include "EffekseerBase.h"

//�G�t�F�N�V�A�̊֐���UpdateEffekseer3D�ƕ`��֌W�̊֐���
//1�e�ɂP�񂾂��Ăяo��
//������Ăяo���Ƃ��̂Ԃ�ꏏ�ɏ��������

EffekseerBase::EffekseerBase(std::string name, VECTOR* pos,float size, float height,float speed,bool loopFlag, bool loadFlag) :
	EffectBase(),
	_effectResourceHandle(ResourceServer::LoadEffekseerEffect(_T(name.c_str()),_T(name.c_str()))),
	_pos(*pos),
	_speed(speed),
	_size(size),
	_loopFlag(loopFlag),
	_height(height)
{
	if (loadFlag) {
		_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
		SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
		SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);
	}
	_height = 0.0f;
};
EffekseerBase::EffekseerBase(int handle, VECTOR* pos, float size, float height, float speed , bool loopFlag, bool loadFlag) :
	EffectBase(),
	_effectResourceHandle(handle),
	_pos(*pos),
	_speed(speed),
	_size(size),
	_loopFlag(loopFlag),
	_height(height)
{
	if (loadFlag) {
		_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
		SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
		SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);
	}
};

EffekseerBase::~EffekseerBase() {
	if (IsEffekseer3DEffectPlaying(_playingEffectHandle) != -1) {
		StopEffekseer3DEffect(_playingEffectHandle);
	}
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
			StopEffekseer3DEffect(_playingEffectHandle);
			_IsPlay = false;
		}
	}

	return true;
};

bool EffekseerBase::Render() {
	return true;
};