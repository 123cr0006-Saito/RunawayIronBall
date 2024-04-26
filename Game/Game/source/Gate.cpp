//----------------------------------------------------------------------
// @filename Gate.h
// @date: 2024/03/15
// @author: saito ko
// @explanation
// �v���C���[���G���΃N���A�ɂȂ�I�u�W�F�N�g�N���X
//----------------------------------------------------------------------
#include "Gate.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param pos �ʒu
// @param r ���a
// @param handle �摜�n���h��
// @param size �摜�n���h���̍ő吔
// @param animSpeed �A�j���[�V�����̍Đ����x
// @param handlesize �r���{�[�h�ŕ\������Ƃ��̉���
// @return ����
//----------------------------------------------------------------------
Gate::Gate(VECTOR pos, int r, int* handle, int size, int animSpeed, int handlesize) :
_pos(pos),
_r(r),
_size(size),
_animSpeed(animSpeed),
_handleSize(handlesize)
{
	_handle = NEW int[_size];
	for (int i = 0; i < _size; i++) {
		_handle[i] = handle[i];
	}
	_animCount = 0;
	_currentTime = GetNowCount();
	_animCount = 0;
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return ����
//----------------------------------------------------------------------
Gate::~Gate() {
	delete[] _handle; _handle = nullptr;
};
//----------------------------------------------------------------------
// @brief �X�V����
// @return ����
//----------------------------------------------------------------------
void Gate::Process(){
	if (GetNowCount() - _currentTime >= _animSpeed) {
		_currentTime = GetNowCount();
		_animCount++;
		if (_animCount >= _size) {
			_animCount = 0;
		}
	}
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return ����
//----------------------------------------------------------------------
void Gate::Draw() {
	DrawBillboard3D(_pos, 0.5f, 0.5f, _handleSize, 0.0f, _handle[_animCount], true);
};