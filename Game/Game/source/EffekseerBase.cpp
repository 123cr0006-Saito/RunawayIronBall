#include "EffekseerBase.h"

//�G�t�F�N�V�A�̊֐���UpdateEffekseer3D�ƕ`��֌W�̊֐���
//1�e�ɂP�񂾂��Ăяo��
//������Ăяo���Ƃ��̂Ԃ�ꏏ�ɏ��������

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