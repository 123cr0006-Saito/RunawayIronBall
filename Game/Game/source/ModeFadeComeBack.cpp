//----------------------------------------------------------------------
// @filename ModeFadeComeBack.cpp
// @date: 2024/03/02
// @author: saito ko
// @explanation
// �X�e�[�W�J�ڎ��Ƀt�F�[�h�C���A�t�F�[�h�A�E�g���s�����߂̃N���X
// ModeFade�ƈႤ�_�͏�Ƀt�F�[�h�C�����Ă���t�F�[�h�A�E�g���A
// �w��̃��[�h���폜���Ȃ���J�ڂ���
//----------------------------------------------------------------------
#include "ModeFadeComeBack.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param Time �t�F�[�h����
// @param modeName ���C���[��ύX���郂�[�h��
// @param layer �w�肵�����[�h�̃��C���[�����̒l�ɕύX
// @param IsProcessSkip �t�F�[�h���v���Z�X���X�L�b�v���邩�ǂ���
//----------------------------------------------------------------------
ModeFadeComeBack::ModeFadeComeBack(int Time, std::string modeName, int layer, bool IsProcessSkip) :ModeFade(Time, false) {
	_fadeEnd = 255;
	_fadeStart = 0;
	_deleteMode = nullptr;
	_changeLayer = layer;
	_changeModeName = modeName;
	_IsProcessSkip = IsProcessSkip;
};
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param Time �t�F�[�h����
// @param mode �폜���郂�[�h��
// @param IsProcessSkip �t�F�[�h���v���Z�X���X�L�b�v���邩�ǂ���
//----------------------------------------------------------------------
ModeFadeComeBack::ModeFadeComeBack(int Time, ModeBase* mode, bool IsProcessSkip) :ModeFade(Time,false){
	_fadeEnd = 255;
    _fadeStart = 0;
	_deleteMode = mode;
	_changeLayer = 0;
	_changeModeName = "";
	_IsProcessSkip = IsProcessSkip;
};
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param Time �t�F�[�h����
// @param mode �폜���郂�[�h��
// // @param modeName ���C���[��ύX���郂�[�h��
// @param layer �w�肵�����[�h�̃��C���[�����̒l�ɕύX
// @param IsProcessSkip �t�F�[�h���v���Z�X���X�L�b�v���邩�ǂ���
//----------------------------------------------------------------------
ModeFadeComeBack::ModeFadeComeBack(int Time, ModeBase* mode, std::string modeName, int layer, bool IsProcessSkip) :ModeFade(Time, false) {
	_fadeEnd = 255;
	_fadeStart = 0;
	_deleteMode = mode;
	_changeLayer = layer;
	_changeModeName = modeName;
	_IsProcessSkip = IsProcessSkip;
};
//----------------------------------------------------------------------
// @brief ������
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeFadeComeBack::Initialize(){
	if (!base::Initialize()) { return false; }
	return true;
};
//----------------------------------------------------------------------
// @brief �I������
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeFadeComeBack::Terminate(){
	base::Terminate();
	_deleteMode = nullptr;
	return true;
};
//----------------------------------------------------------------------
// @brief �X�V����
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeFadeComeBack::Process(){
	base::Process();
	// �v���Z�X���X�L�b�v����ꍇ
	if(_IsProcessSkip){
	   ModeServer::GetInstance()->SkipProcessUnderLayer();
	}
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	int nowTime = GetNowCount() - _currentTime;
	// �t�F�[�h����
	_alphaFade = Easing::Linear(nowTime, _fadeStart, _fadeEnd, _fadeTime);
	// �t�F�[�h�I��
	if (_alphaFade >= 255) {
		// �l�̓���ւ�
		_alphaFade = _fadeEnd;

		int temp = _fadeStart;
        _fadeStart = _fadeEnd;
        _fadeEnd = temp;
		_currentTime = GetNowCount();

		// �폜���郂�[�h������Ƃ�
		if(_deleteMode != nullptr){
		   ModeServer::GetInstance()->Del(_deleteMode);
	    }
		// ���C���[��ύX����ꍇ
		if(_changeModeName != ""){
			ModeServer::GetInstance()->ChangeLayer(_changeModeName, _changeLayer);
		}
	}
	else if (_alphaFade < 0) {
		// �t�F�[�h�I�� �폜
		ModeServer::GetInstance()->Del(this);
	}

	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �������Ă��邩
//----------------------------------------------------------------------
bool ModeFadeComeBack::Render() {
	base::Render();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alphaFade);
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, _alphaFade);
	return true;
};