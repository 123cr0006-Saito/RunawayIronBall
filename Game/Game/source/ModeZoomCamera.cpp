//----------------------------------------------------------------------
// @filename ModeZoomCamera.cpp
// ��date: 2024/03/09
// ��author: saito ko
// @explanation
// �Q�[�g�\�����ɃJ�������ړ�������N���X
//----------------------------------------------------------------------
#include "ModeZoomCamera.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param pos �J�����̈ʒu
//----------------------------------------------------------------------
ModeZoomCamera::ModeZoomCamera(VECTOR pos) {
	_pos = pos;
	_camera = nullptr;
	_time = 0;
	_currentTime = 0;
};

bool ModeZoomCamera::Initialize(){
	int time = 5 * 1000;
	_time = time + 2000; // �ړ����Ԃ�2�b�ǉ�
	_currentTime = GetNowCount();
	//�^�[�Q�b�g�̈ʒu
	VECTOR targetPos = _pos; 
	// �J�����̎��̕���
	 VECTOR camPos= GetCameraPosition();//�J�����̈ʒu�擾
	 VECTOR nextDirVec = VSub(_pos, camPos);// �Q�[�g�̈ʒu�i���_�j�Ɍ��������x�N�g���쐬
	 nextDirVec = VNorm(nextDirVec);//���K��
	 //�J�����̍��̕���
	 VECTOR nowDirVec = VSub(camPos, Player::GetInstance()->GetPosition());
	 nowDirVec = VNorm(nowDirVec);//���K��
	 // �J�����̎��̈ʒu
	 VECTOR nextCameraPos = VAdd(VScale(nextDirVec, -1000), VAdd(_pos,VGet(0, 1000, 0)));//�Q�[�g�̈ʒu���班���ʒu�𗣂��č����𑫂�

	_camera = NEW TargetZoomCamera(targetPos,nextCameraPos,nowDirVec,nextDirVec,time);
	return true;
};

bool ModeZoomCamera::Terminate() {
	delete _camera; _camera = nullptr;
	return true;
}

bool ModeZoomCamera::Process() {
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();

	int nowTime = GetNowCount() - _currentTime;
	_camera->Process();

	if (nowTime >= _time) {
		ModeServer::GetInstance()->Del(this);
	}

	return true;
};