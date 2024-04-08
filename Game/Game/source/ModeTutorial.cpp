//----------------------------------------------------------------------
// @filename ModeTutorial.cpp
// ��date: 2024/03/17
// ��author: saito ko
// @explanation
// �X�e�[�W�̍ŏ��Ƀ`���[�g���A����ʂ�\������N���X
//----------------------------------------------------------------------
#include "ModeTutorial.h"
#include "TimeLimit.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param handle �`���[�g���A���摜�̃n���h��
// @param size �`���[�g���A���摜�̐�
//----------------------------------------------------------------------
ModeTutorial::ModeTutorial(int* handle, int size){
	_pageSize = size;
	_handle = NEW int[_pageSize];
	for(int i = 0; i < _pageSize; i++){
		_handle[i] = handle[i];
	}
	_selectItem = 0;
	_input = XInput::GetInstance();
};
//----------------------------------------------------------------------
// @brief ����������
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeTutorial::Initialize(){
	TimeLimit::GetInstance()->Stop();
	return true;
};
//----------------------------------------------------------------------
// @brief �I������
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeTutorial::Terminate(){
	delete[] _handle;
	_input = nullptr;
	TimeLimit::GetInstance()->Restart();
	return true;
};
//----------------------------------------------------------------------
// @brief �X�V����
// @return �������Ă��邩
bool ModeTutorial::Process(){
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	// ���̉摜��
	if (_input->GetTrg(XINPUT_BUTTON_A) ) {
		if (_selectItem < _pageSize) {
			_selectItem++;
			if (_selectItem == _pageSize) {
				ModeServer::GetInstance()->Del(this);
			}
		}
	}

	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �������Ă��邩
bool ModeTutorial::Render(){
	// ����mode�����mode�������ꍇ�A�`���[�g���A���摜�̕`��
	if (!ModeServer::GetInstance()->IsAboutLayer(this)) {
		int x, y;
		GetGraphSize(_handle[_selectItem], &x, &y);
		int ScreenX, ScreenY, ScreenDepth;
		GetScreenState(&ScreenX, &ScreenY, &ScreenDepth);
		DrawGraph((ScreenX - x) / 2, (ScreenY - y) / 2, _handle[_selectItem], true);
	}
	return true;
};
