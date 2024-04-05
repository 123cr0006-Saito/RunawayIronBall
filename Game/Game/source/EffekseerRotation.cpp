//----------------------------------------------------------------------
// @filename EffekseerRotation.cpp
// ��date: 2024/03/07
// ��author: saito ko
// @explanation
// ��]�U���̃G�t�F�N�g���Đ�����N���X
//----------------------------------------------------------------------
#include "EffekseerRotation.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param handle �G�t�F�N�V�A�n���h��
// @param pos �Đ��ʒu
// @param size �G�t�F�N�g�̊g�嗦
// @param rotation �G�t�F�N�g�̉�]�l
// @param height �G�t�F�N�g�Đ��ʒu����+y�����ɂ��炷����
// @param speed �G�t�F�N�g�̍Đ����x
// @param loopFlag ���[�v�t���O
// @return ����
//----------------------------------------------------------------------
EffekseerRotation::EffekseerRotation(int handle, VECTOR* pos, float size, VECTOR* rotation,float height, float speed, bool loopFlag) :
	EffekseerBase(handle, pos, size,height,1.0f,false,false),
	_pos(pos),
	_rotation(rotation)
{
	_currentTime = GetNowCount();
	VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
	VECTOR Rotation = *(_rotation);
	Rotation.y = 0.0f;
	float angle = Math::CalcVectorAngle(vBase, Rotation);
	angle *= Rotation.x < 0.0f ? 1.0f : -1.0f;

	float angleList[_maxEffect] = { Math::DegToRad(5),Math::DegToRad(-5) };

	for (int i = 0; i < _maxEffect; i++) {
		_playingEffectHandle[i] = PlayEffekseer3DEffect(_effectResourceHandle);
		SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle[i], _speed);
		SetScalePlayingEffekseer3DEffect(_playingEffectHandle[i], _size, 10, _size);
		SetRotationPlayingEffekseer3DEffect(_playingEffectHandle[i], 0, angle + angleList[i], 0);
	}
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return ����
//----------------------------------------------------------------------
EffekseerRotation::~EffekseerRotation() {
	for (int i = 0; i < _maxEffect; i++) {
		if (IsEffekseer3DEffectPlaying(_playingEffectHandle[i]) != -1) {
			StopEffekseer3DEffect(_playingEffectHandle[i]);
		}
	}
};
//----------------------------------------------------------------------
// @brief �X�V����
// @return �����������ǂ���
//----------------------------------------------------------------------
bool EffekseerRotation::Process() {

	float endTime = 8.0f/ 60.0f * 1000;

	VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
	VECTOR Rotation = *(_rotation);
	Rotation.y = 0.0f;
	float angle = Math::CalcVectorAngle(vBase, Rotation);
	angle *= Rotation.x < 0.0f ? 1.0f : -1.0f;

	float slippage = 2.0f;
	float angleList[_maxEffect] = { Math::DegToRad(slippage),Math::DegToRad(-slippage) };

	for (int i = 0; i < _maxEffect; i++) {
		SetPosPlayingEffekseer3DEffect(_playingEffectHandle[i], (*_pos).x, (*_pos).y + _height, (*_pos).z);

		SetRotationPlayingEffekseer3DEffect(_playingEffectHandle[i], 0, angle + angleList[i], 0);

		if (GetNowCount() - _currentTime >= endTime) {
			if (_loopFlag) {
				_playingEffectHandle[i] = PlayEffekseer3DEffect(_effectResourceHandle);
				SetSpeedPlayingEffekseer3DEffect(_playingEffectHandle[i], _speed);
				SetScalePlayingEffekseer3DEffect(_playingEffectHandle[i], _size, _size, _size);
			}
			else {
				StopEffekseer3DEffect(_playingEffectHandle[i]);
				_IsPlay = false;
			}
		}
	}

	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool EffekseerRotation::Render() {
	return true;
};