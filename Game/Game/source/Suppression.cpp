//----------------------------------------------------------------------
// @filename Suppression.cpp
// @date: 2024/02/26
// @author: saito ko
// @explanation
// �G�⌚����j�󂵂����Ɍ������Ă��������Q�[�W���Ǘ�����N���X
//----------------------------------------------------------------------
#include "Suppression.h"
Suppression* Suppression::_instance = nullptr;
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @return ����
//----------------------------------------------------------------------
Suppression::Suppression(){
	_instance = this;
	_maxSuppression = 0;
	_nowSuppression = 0;
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return ����
//----------------------------------------------------------------------
Suppression::~Suppression() {
	_instance = nullptr;
};
//----------------------------------------------------------------------
// @brief �����l��ǉ�
// @param suppression �ǉ����鐧���l
// @return ����
//----------------------------------------------------------------------
void Suppression::AddSuppression(int suppression) { 
	_maxSuppression += suppression;
	_nowSuppression += suppression;
}
//----------------------------------------------------------------------
// @brief �����l������
// @param suppression �������鐧���l
// @return ����
//----------------------------------------------------------------------
void Suppression::SubSuppression(int suppression) { if(_nowSuppression > 0)_nowSuppression -= suppression; }
//----------------------------------------------------------------------
// @brief �����l���폜
// @return �����l
//----------------------------------------------------------------------
void Suppression::ClearSuppression() {
	_maxSuppression = 0;
	_nowSuppression = 0;
};
//----------------------------------------------------------------------
// @brief�����l�̊�������20%�ȉ����ǂ�����Ԃ�
// @return 20%�ȉ���
//----------------------------------------------------------------------
bool Suppression::GetIsRatioBut20PercentLess() {
	// max��20%�ȉ��Ȃ�true
	if (_nowSuppression <= static_cast<float>(_maxSuppression / 10 * (10 - 8))) {
		return true;
	}
	return false;
}