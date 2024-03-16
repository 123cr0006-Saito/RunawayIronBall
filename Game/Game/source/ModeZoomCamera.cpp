#include "ModeZoomCamera.h"
bool ModeZoomCamera::Initialize(){
	int time = 5 * 1000;
	_time = time + 2000; // �ړ����Ԃ�2�b�ǉ�
	_currentTime = GetNowCount();
	//�^�[�Q�b�g�̈ʒu
	VECTOR targetPos = VGet(0, 0, 0);// �Q�[�g�̈ʒu
	// �J�����̎��̕���
	 VECTOR camPos= GetCameraPosition();//�J�����̈ʒu�擾
	 VECTOR nextDirVec = VSub(VGet(0, 0, 0), camPos);// �Q�[�g�̈ʒu�i���_�j�Ɍ��������x�N�g���쐬
	 nextDirVec = VNorm(nextDirVec);//���K��
	 //�J�����̍��̕���
	 VECTOR nowDirVec = VSub(camPos, Player::GetInstance()->GetPosition());
	 nowDirVec = VNorm(nowDirVec);//���K��
	 // �J�����̎��̈ʒu
	 VECTOR nextCameraPos = VAdd(VScale(nextDirVec, -1000), VGet(0, 1000, 0));//�Q�[�g�̈ʒu���班���ʒu�𗣂��č����𑫂�

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