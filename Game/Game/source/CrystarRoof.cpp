#include "CrystarRoof.h"
CrystarRoof::CrystarRoof(int model, int parent,std::string frameName) {
	_model = model;
    _parent = parent;
	_frame = MV1SearchFrame(parent,"Hip");
	_joint = frameName;
	Update();
};

CrystarRoof:: ~CrystarRoof() {

};

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

bool CrystarRoof::Render() {
	MV1DrawModel(_model);
	return true;
};