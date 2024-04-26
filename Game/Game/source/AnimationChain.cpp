//----------------------------------------------------------------------
// @filename AnimationChain.cpp
// @date: 2024/03/16
// @author: saito ko
// @explanation
// �Q�[���N���A���̃��U���g��ʂō��̃A�j���[�V������\������N���X
//----------------------------------------------------------------------
#include "AnimationChain.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param pos: ���̏I�_�̍��W
// @param angle: ���̊p�x
// @return �Ȃ�
//----------------------------------------------------------------------
AnimationChain::AnimationChain(VECTOR pos,float angle){
	// ������
	// �摜�̓ǂݍ���
	_handle = ResourceServer::LoadGraph("UI_Chain","res/UI/Chain/Kusari_Straight.png");
	_frameCount = 0;
	_maxCount = 60;
	_endX = pos.x;
	_endY = pos.y;
	_angle = angle * DX_PI / 180;
	float distance = 10000;
	_startX = pos.x - cos(_angle) * distance;
	_startY = pos.y - sin(_angle) * distance;
	_nowX = _startX;
	_nowY = _startY;
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return �Ȃ�
//----------------------------------------------------------------------
AnimationChain::~AnimationChain(){
};
//----------------------------------------------------------------------
// @brief ���C������
// @return �Ȃ�
//----------------------------------------------------------------------
void AnimationChain::Process(){
	// �t���[���J�E���g���ő�l�ɒB����܂ŁA���W���X�V
	if (_frameCount < _maxCount) {
		_nowX = _startX + (_endX - _startX) * _frameCount / _maxCount;
		_nowY = _startY + (_endY - _startY) * _frameCount / _maxCount;
		_frameCount++;
	}
};
//----------------------------------------------------------------------
// @brief �I������
// @return �Ȃ�
//----------------------------------------------------------------------
void AnimationChain::Draw(){
	// �摜�̕`�� ���̉摜���傫���������� 0.8�{�ɏC��
	DrawRotaGraph(_nowX, _nowY, 0.8f, _angle, _handle, true);
};