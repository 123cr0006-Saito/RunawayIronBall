//----------------------------------------------------------------------
// @filename CrystarRoof.cpp
// @date: 2024/02/24
// @author: saito ko
// @explanation
// �N���C�X�^�[�̉���������`�悷�邽�߂̃N���X
//----------------------------------------------------------------------
#include "CrystarRoof.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param model ���f���n���h��
// @param parent �e���f���n���h��
// @param frameName ������z�u����ʒu���������邽�߂̃t���[����
// @return ����
//----------------------------------------------------------------------
CrystarRoof::CrystarRoof(int model, int parent,std::string frameName) {
	_model = model;
    _parent = parent;
	_frame = MV1SearchFrame(parent,"Hip");
	_joint = frameName;
	Update();
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return ����
//----------------------------------------------------------------------
CrystarRoof:: ~CrystarRoof() {};
//----------------------------------------------------------------------
// @brief �X�V
// @return ����
//----------------------------------------------------------------------
bool CrystarRoof::Update(){
	// �s��̏�����
	MV1SetMatrix(_model, MGetIdent());

	// �s��̍X�V
	// ���W�̎擾
	int frame = MV1SearchFrame(_model, _joint.c_str());
	VECTOR framePos = MV1GetFramePosition(_model, frame);
	// �擾�������W�����_�ɂ��邽�߂̕��s�ړ��s����쐬
	MATRIX transMatrix = MGetTranslate(VScale(framePos,-1.0f));
	// �e�t���[���̃A�^�b�`����ꏊ�̍s����擾
    MATRIX frameMatrix = MV1GetFrameLocalWorldMatrix(_parent,_frame);

	//�s�������
	MATRIX MixMatrix = MMult(transMatrix,frameMatrix);

	// ���������s����A�^�b�`���郂�f���ɃZ�b�g
	MV1SetMatrix(_model, MixMatrix);
	return true;
};
//----------------------------------------------------------------------
// @brief �`��
// @return ����
//----------------------------------------------------------------------
bool CrystarRoof::Render() {
	MV1DrawModel(_model);
	return true;
};