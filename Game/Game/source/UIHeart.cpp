//----------------------------------------------------------------------
// @filename UIHeart.cpp
// @date: 2023/12/25
// @author: saito ko
// @explanation
// �v���C���[HP��UI��\������N���X
//----------------------------------------------------------------------
#include "UIHeart.h"
const int UIHeart::_damageSeconds = 3000;
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param pos �ʒu
// @param size �摜�̍ő喇��
// @param handle �摜�̃n���h��
// @param damageHandleNum �_���[�W���̉摜�̃n���h��
// @return ����
//----------------------------------------------------------------------
UIHeart::UIHeart(VECTOR pos, int size, int* handle, int damageHandleNum) : UIBase::UIBase(pos, size, handle)
{
	//_hp = 4;
	_player = Player::GetInstance();
	float _x, _y;
	GetGraphSizeF(_handle[_handleNum], &_x, &_y);
	_heart = NEW DrawGauge(_handle[damageHandleNum], 3, _x, true);
	_currentTime = 0;
	_oldHp = 4;
	_IsDamage = false;
}
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return ����
//----------------------------------------------------------------------
UIHeart::~UIHeart() {
	delete _heart; _heart = nullptr;
	if (_handle != nullptr) {
		delete[] _handle; _handle = nullptr;
	}
};
//----------------------------------------------------------------------
// @brief �_���[�W���󂯂����̏���
// @param hp �v���C���[��HP
// @return ����
//----------------------------------------------------------------------
void UIHeart::SetDamage(int hp) {
	if (_oldHp != hp) {
		_IsDamage = true;
		_currentTime = GetNowCount();
	}
	_oldHp = hp;
};
//----------------------------------------------------------------------
// @brief �X�V����
// @return �����������ǂ���
//----------------------------------------------------------------------
bool  UIHeart::Process() {
	int hp = _player->GetHP();
	SetDamage(hp);

	if (_IsDamage) {
		float _x, _y;
		GetGraphSizeF(_handle[_handleNum], &_x, &_y);
		int nowTime = GetNowCount() - _currentTime;
		VECTOR gaugePos = VAdd(_pos, VGet(_x * hp + _x / 2, _y / 2, 0));
		_heart->Process(gaugePos, _damageSeconds, _damageSeconds);
		if (_damageSeconds < nowTime) {
			_IsDamage = false;
		}
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool  UIHeart::Draw() {
	int _x, _y;
	int hp = _player->GetHP();
	//�̗͂̕\��
	GetGraphSize(_handle[_handleNum], &_x, &_y);
	for (int i = 0; i < 4; i++) {
		i >= hp ? _handleNum = 1 : _handleNum = 0;
		if (i != hp || !_IsDamage) {
			DrawGraph(_pos.x + _x * i, _pos.y, _handle[_handleNum], true);
		}
	}
	//�����Ă���̗͂̕\��
	if (_IsDamage && hp < 4) {
		int nowTime = GetNowCount() - _currentTime;
		if (nowTime % 500 > 250) {
			_heart->Draw();
		}
	};
	return true;
};