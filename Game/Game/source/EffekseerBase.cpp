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
//----------------------------------------------------------------------
// @breif:�R���X�g���N�^
// @param: name �G�t�F�N�g�̃t�@�C���p�X
// @param: pos �G�t�F�N�g�̍��W
// @param: size �G�t�F�N�g�̔{��
// @param: height ��_���炸�炷����
// @param: speed �Đ����x
// @param: loopFlag ���[�v���邩�ǂ���
// @param: loadFlag �ǂݍ��݂����邩�ǂ���
// @return:�Ȃ�
//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
// @breif:�R���X�g���N�^
// @param: handle �G�t�F�N�g�̃n���h��
// @param: pos �G�t�F�N�g�̍��W
// @param: size �G�t�F�N�g�̔{��
// @param: height ��_���炸�炷����
// @param: speed �Đ����x
// @param: loopFlag ���[�v���邩�ǂ���
// @param: loadFlag �ǂݍ��݂����邩�ǂ���
// @return:�Ȃ�
//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
// @breif:�f�X�g���N�^
// @return:�Ȃ�
//----------------------------------------------------------------------
EffekseerBase::~EffekseerBase() {
	StopEffekseer3DEffect(_playingEffectHandle);
};
//----------------------------------------------------------------------
// @breif:�X�V����
// @return:�����������ǂ���
//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
// @breif:�`�揈��
// @return: �����������ǂ���
//----------------------------------------------------------------------
bool EffekseerBase::Render() {
	return true;
};