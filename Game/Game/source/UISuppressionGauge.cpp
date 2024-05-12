//----------------------------------------------------------------------
// @filename UISuppressionGauge.cpp
// @date: 2024/02/26
// @author: saito ko
// @explanation
// �����Q�[�W��\������N���X
//----------------------------------------------------------------------
#include "UISuppressionGauge.h"

const unsigned short UISuppressionGauge::vertex[6]{ 0,1,2,2,1,3 };
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param pos �ʒu
// @param size �摜�̍ő喇��
// @param handle �n���h��
// @return ����
//----------------------------------------------------------------------
UISuppressionGauge::UISuppressionGauge(VECTOR pos, int size, int* handle) : UIBase::UIBase( pos,  size,  handle){
	_suppressionValue = Suppression::GetInstance();
	//     x,      y,    u,    v
	float posTbl[][4] = {
		{-_cx,-_cy,0.0f,0.0f},
		{_cx,-_cy,1.0f,0.0f},
		{-_cx,_cy,0.0f,1.0f},
		{_cx,_cy,1.0f,1.0f}
	};

	VECTOR center = VAdd(pos, VGet(_cx, _cy, 0));
	// �����Q�[�W�̒��S���W��ݒ�
	_standardPos = VAdd(center, VGet(posTbl[0][0], posTbl[0][1], 0));
	// �����Q�[�W�̏�����
	for (int i = 0; i < 4; i++) {
		//���������̃Q�[�W
		_suppression[i].pos = VAdd(center, VGet(posTbl[i][0], posTbl[i][1], 0));
		_suppression[i].u = posTbl[i][2];
		_suppression[i].v = posTbl[i][3];
		_suppression[i].dif = GetColorU8(255, 255, 255, 255);
		_suppression[i].rhw = 1.0f;
		//�c��̃Q�[�W
		_residue[i].pos = VAdd(center, VGet(posTbl[i][0], posTbl[i][1], 0));
		_residue[i].u = posTbl[i][2];
		_residue[i].v = posTbl[i][3];
		_residue[i].dif = GetColorU8(125, 125, 125, 255);
		_residue[i].rhw = 1.0f;
	}

	GetGraphSizeF(handle[2], &_ironHandleSize.first.x, &_ironHandleSize.first.y);
	_ironHandleSize.second = _cy / 2;
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return ����
//----------------------------------------------------------------------
UISuppressionGauge::~UISuppressionGauge() {
	_suppressionValue = nullptr;
	if (_handle != nullptr) {
		delete[] _handle; _handle = nullptr;
	}
};
//----------------------------------------------------------------------
// @brief �X�V����
// @return �������Ă��邩
//----------------------------------------------------------------------
bool UISuppressionGauge::Process(){
	int nowSuppression = _suppressionValue->GetNowSuppression();
	int maxSuppression = _suppressionValue->GetMaxSuppression();

	float ratio = static_cast<float>(nowSuppression) / maxSuppression;

	// �^�񒆂̒��_2�̍��W�Ƃ���ς���������ł���
	for (int i = 0; i < 2; i++) {
		_suppression[0 + 2 * i].pos = VAdd(_standardPos, VScale(VGet(_cx * ratio, _cy * i, 0),2));
		_suppression[0 + 2 * i].u =  ratio;
		_residue[1 + 2 * i].pos = VAdd(_standardPos, VScale(VGet(_cx * ratio, _cy * i, 0), 2));
		_residue[1 + 2 * i].u = ratio;
	}
	_ironHandleSize.first = VAdd(_standardPos, VScale(VGet(_cx * ratio, 0 , 0), 2));
	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �������Ă��邩
//----------------------------------------------------------------------
bool UISuppressionGauge::Draw() {
	VERTEX2D* list[2] = { _suppression,_residue};
	// �����Q�[�W�̕`��
	for (int i = 0; i < 2; i++) {
		DrawPrimitiveIndexed2D(list[i], 4, vertex, 6, DX_PRIMTYPE_TRIANGLELIST, _handle[i], true);
	}
	// �S���̕`��
	DrawRotaGraph(_ironHandleSize.first.x, _ironHandleSize.first.y + _ironHandleSize.second, 0.5f,0.0f,_handle[2], true);
	return true;
};