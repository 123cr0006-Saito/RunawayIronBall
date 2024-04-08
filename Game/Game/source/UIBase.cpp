//----------------------------------------------------------------------
// @filename UIBase.cpp
// ��date: 2023/12/19
// ��author: saito ko
// @explanation
// UI�̊��N���X
//----------------------------------------------------------------------
#include "UIBase.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param pos �ʒu
// @return ����
//----------------------------------------------------------------------
UIBase::UIBase(VECTOR pos) {
	_pos = pos;
	_handleNum = 0;
	_cx = 0;
	_cy = 0;
};
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param pos �ʒu
// @param size �n���h���̐�
// @param handle �n���h��
UIBase::UIBase(VECTOR pos, int size, int* handle) {
	_handle = NEW int[size];
	for (int i = 0; i < size; i++) {
		_handle[i] = handle[i];
	}
	_pos = pos;
	_handleNum = 0;
	float _x, _y;
	GetGraphSizeF(_handle[_handleNum], &_x, &_y);
	_cx = _x / 2;
	_cy = _y / 2;
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return ����
//----------------------------------------------------------------------
UIBase::~UIBase() {
	if (_handle != nullptr) {
		delete[] _handle; _handle = nullptr;
	}
};
//----------------------------------------------------------------------
// @brief �X�V����
// @return �����������ǂ���
//----------------------------------------------------------------------
bool UIBase::Process() {
	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool UIBase::Draw() {
	float _cx, _cy;
	GetGraphSizeF(_handle[0], &_cx, &_cy);
	
	_cx = _cx /= 2;
	_cy = _cy /= 2;

	DrawGraphF(_pos.x - _cx, _pos.y - _cy, _handle[_handleNum], true);
	return true;
};