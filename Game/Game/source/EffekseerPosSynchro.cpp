//----------------------------------------------------------------------
// @filename EffekseerPosSynchro.h
// ��date: 2024/02/8
// ��author: saito ko
// @explanation
// �ʒu���̂���A�h���X���󂯎��ړ�����G�t�F�N�V�A���Đ�����N���X
//----------------------------------------------------------------------
#include "EffekseerPosSynchro.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param name �ǂݍ��ݎ��̃t�@�C����
// @param pos �G�t�F�N�g�̍Đ��ʒu
// @param size �G�t�F�N�g�̊g��{��
// @param rotation �G�t�F�N�g�̉�]
// @param height �G�t�F�N�g�̈ʒu����+y�������ɂ��炷����
// @param speed �G�t�F�N�g�̍Đ����x
// @param loopFlag ���[�v���邩�ǂ���
// @return ����
//----------------------------------------------------------------------
EffekseerPosSynchro::EffekseerPosSynchro(std::string name, VECTOR* pos, float size, VECTOR rotation, float height, float speed, bool loopFlag) :
	base(name, pos, size, height, speed, loopFlag, false),
	_pos(pos)
{
	_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
	SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
	SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);

	VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
	rotation.y = 0.0f;
	float angle = Math::CalcVectorAngle(vBase, rotation);
	angle *= rotation.x < 0.0f ? 1.0f : -1.0f;
	SetRotationPlayingEffekseer3DEffect(_playingEffectHandle, 0, angle, 0);
};
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param handle �ǂݍ��ݎ��̃t�@�C����
// @param pos �G�t�F�N�g�̍Đ��ʒu
// @param size �G�t�F�N�g�̊g��{��
// @param rotation �G�t�F�N�g�̉�]
// @param height �G�t�F�N�g�̈ʒu����+y�������ɂ��炷����
// @param speed �G�t�F�N�g�̍Đ����x
// @param loopFlag ���[�v���邩�ǂ���
// @return ����
//----------------------------------------------------------------------
EffekseerPosSynchro::EffekseerPosSynchro(int handle, VECTOR* pos, float size, VECTOR rotation, float height, float speed, bool loopFlag) :
	base(handle, pos, size, height, speed, loopFlag, false),
_pos(pos)
{
	_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
	SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle, _speed);
	SetScalePlayingEffekseer3DEffect(_playingEffectHandle, _size, _size, _size);

	VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
	rotation.y = 0.0f;
	float angle = Math::CalcVectorAngle(vBase, rotation);
	angle *= rotation.x < 0.0f ? 1.0f : -1.0f;
	SetRotationPlayingEffekseer3DEffect(_playingEffectHandle, 0, angle, 0);
	
	_currentTime = GetNowCount();
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return ����
//----------------------------------------------------------------------
EffekseerPosSynchro::~EffekseerPosSynchro() {
	if (IsEffekseer3DEffectPlaying(_playingEffectHandle) != -1) {
		StopEffekseer3DEffect(_playingEffectHandle);
	}
};
//----------------------------------------------------------------------
// @brief �X�V����
// @return �����������ǂ���
//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool EffekseerPosSynchro::Render() {
	return true;
};