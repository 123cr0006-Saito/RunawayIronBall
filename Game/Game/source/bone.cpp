#include "bone.h"

const Vector3D bone::_orign(0,0,0);

const float bone::_massWeight = 1.5f;
const float bone::_viscousResistance = 20.0f;
const float bone::_gravity = 4000.0f;
const float bone::_spring = 800.0f;

const float bone::_naturalCorrectionFactor = 0.8f;
const Vector3D bone::_gravityDir(0.0f, -1.0f, 0.0f);

//1�e ���0.015 ~ 0.017�b���炢 
//1�񂾂Ɣ��U����
//2�񂾂Ƃ��ɂႮ�ɂ�ɂȂ�
//10�񂾂ƍ����ۂ��Ȃ�
//100��ȏオ���ꂢ�Ɍ�����

//���͑��150�`170��
const double bone::_processInterval = 0.0001;

bone::bone(
	int* Model,
	std::vector<int> list,
	int size
) :
	_model(Model),
	_frameList(list),
	_listSize(size),
	_springList(size + 1),
	_naturalList(size + 1)
{
	//�{�[���̏�����
	_vecDirList = new Vector3D[_listSize];
	_orignPos = new Vector3D[_listSize + 2];
	_transMatrixList = new MATRIX[_listSize];


	for (int i = 0; i < _listSize; i++) {
		_transMatrixList[i] = MV1GetFrameLocalMatrix(*_model, _frameList[i + 1]);
		MATRIX local_mat = MV1GetFrameLocalMatrix(*_model, _frameList[i + 2]);
		_vecDirList[i] = VTransform(_orign.toVECTOR(), local_mat);

		for (int j = 0; j < 3; j++) {
			_transMatrixList[i].m[3][j] = 0.0f;
		}

		_orignPos[i + 2] = MV1GetFramePosition(*_model, _frameList[i + 2]);
	}
	_orignPos[0] = MV1GetFramePosition(*_model, _frameList[0]);
	_orignPos[1] = MV1GetFramePosition(*_model, _frameList[1]);

	//----------------------------------------------------------------------------------
	//�������Z�����邽�߂̕ϐ��̏�����
	_massPointSize = _frameList.size() - 1;
	_massPosList = new Vector3D[_massPointSize];
	_massAccelList = new Vector3D[_massPointSize];

	for (int i = 0; i < _massPointSize; i++) {
		_massPosList[i].Set(MV1GetFramePosition(*_model, _frameList[i + 1]));
	}

	for (int i = 0; i < _massPointSize - 1; i++) {
		//_naturalList�̏�����
		//_natulalCorrectionFactor�͒ʏ�1.0�ŗǂ��Ǝv�����A
		// ���̖т��ُ�ɒ����Ȃ����Ƃ���_natulalCorrectionFactor��ύX��
		// ���̃��f���̔��̒����ɋ߂���Ԃɂ���
		_naturalList[i] = _naturalCorrectionFactor * (_massPosList[i + 1] - _massPosList[i]).Len();

		//_springList�̏�����
		//�΂˂̒l�͌��܂��Ă��邪_naturalCorrectionFactor�ɔ�Ⴕ�ĕύX����
		//����͎��_�����̒��������K�v�Ȃ���
		// ���_�ɍ��킵�Ă΂˒萔���ύX�\
		_springList[i] = _naturalList[i] * _spring;
	}
};

bone::~bone() {
	_model = nullptr;
	delete[] _vecDirList;         _vecDirList = nullptr;
	delete[] _orignPos;           _orignPos = nullptr;
	delete[] _transMatrixList; _transMatrixList = nullptr;
	delete[] _massPosList;    _massPosList = nullptr;
	delete[] _massAccelList;  _massAccelList = nullptr;
};

void bone::SetMain(Vector3D* pos_list) {
	for (int i = 0; i < _listSize; i++) {
		SetBoneDir(
			pos_list[i + 1],
			pos_list[i],
			_frameList[i + 1],
			_frameList[i],
			_transMatrixList[i],
			_vecDirList[i]
		);
	};
};

void bone::SetBoneDir(
	Vector3D world_dir_vec,
	Vector3D boon_pos,
	int target_frame,
	int parent_frame,
	MATRIX trans_mat,
	Vector3D dir_parent)
{
	MATRIX tmpMat;
	tmpMat = MV1GetFrameLocalWorldMatrix(*_model, parent_frame); // �e�{�[���̃��[�J�������[���h
	tmpMat = MInverse(tmpMat); // �t�s����v�Z
	Vector3D localDirVec = VTransform(world_dir_vec.toVECTOR(), tmpMat);//�{�[��������̃��[�J�����W
	Vector3D localBonePos = VTransform(boon_pos.toVECTOR(), tmpMat);//�����̕t�����̃��[�J�����W

	//�{�[����������������
	Vector3D localDirLook = (localDirVec - localBonePos).Normalize();

	//���̌������玟�̌����ւ̉�]�s��
	MATRIX rotationMat = MGetRotVec2(dir_parent.toVECTOR(), localDirLook.toVECTOR());

	//�����̕t�������ʒu�ɂȂ�悤�A���s�ړ�
	MATRIX shiftPos = trans_mat;
	shiftPos.m[3][0] = localBonePos.x;
	shiftPos.m[3][1] = localBonePos.y;
	shiftPos.m[3][2] = localBonePos.z;

	tmpMat = MMult(rotationMat, shiftPos);
	MV1SetFrameUserLocalMatrix(*_model, target_frame, tmpMat);
}

bool bone::Process() {

	double _elapsedTime = global._timer->GetElapsedTime();
	while (1)
	{
		//1�t���[����_processInterval�ō���������
		if (_elapsedTime < _processInterval)break;
		_elapsedTime -= _processInterval;
		UpdatePosAndAccel(_processInterval);
	}

	return true;
};

//�Q�l�T�C�g
//http://www.den.t.u-tokyo.ac.jp/ad_prog/ode/ //�I�C���[�@�ɂ���
//https://high-school-physics.com/spring-constant-of-the-combined-spring/ //�΂˂̂荇���ɂ���
//�I�C���[�@�Ōv�Z�������߁A�ЂƂO�̌v�Z�����l���g�p���Ă���
//���Ԃ�����΃����Q�N�b�^�@�ɕύX������
void bone::UpdatePosAndAccel(double _elapsedTime) {
	//���Ԃŏ������ו����������������ɋ߂Â��Ă���
	static Vector3D* newPosList = new Vector3D[_massPointSize];
	static Vector3D* newAccelList = new Vector3D[_massPointSize];

	//�t�����̈ʒu�͌Œ�
	_massPosList[0] = MV1GetFramePosition(*_model, _frameList[1]);
	newPosList[0] = _massPosList[0];

	// ���x�ƈʒu�̍X�V
	for (int i = 1; i < _massPointSize; i++) {
		// �j���[�g���̉^�����������
		// F = ma ����͑��x���~�����̂� a = F/m
		Vector3D Accel = ForceWorksToMassPoint(i, _massPosList, _massAccelList) / _massWeight;
		//���x���o��   y(i+1) = y(i) + hf 
		newAccelList[i] = _massAccelList[i] + _elapsedTime * Accel;
		//�ʒu�̍X�V   y(i+1) = y(i) + hf 
		newPosList[i] = _massPosList[i] + _elapsedTime * _massAccelList[i];
	}

	// ���x�ƈʒu���܂Ƃ߂ĕύX
	std::swap(_massAccelList, newAccelList);
	std::swap(_massPosList, newPosList);
};

//���_�ɓ����͂��v�Z F=ma�����߂�
//�Q�l�T�C�g
//https://www.yukimura-physics.com/entry/dyn-f22 //�^���������̗��ĕ��ɂ���
Vector3D bone::ForceWorksToMassPoint(int i, Vector3D* posList, Vector3D* accelList) {
	Vector3D force;

	//�s������
	//�΂˂̂荇�� ������̒e���͂Ɖ������̏d�͂��荇�����v�Z
	//��R������
	//�d�͂�������

	//���_i�`���_i+1�Ԃ̂΂˂���󂯂��
	if (i < _massPointSize - 1) {
		////�΂˂̐L�ы�𒲂ׂ�
		float growth = (posList[i + 1] - posList[i]).Len() - _naturalList[i];
		//�΂˂̐L�т�͂ɕϊ�
		force += _springList[i] * growth * (posList[i + 1] - posList[i]).Normalize();
	}

	// ���_i-1�`���_i�Ԃ̃o�l����󂯂�� 
	//�΂˂̐L�ы�𒲂ׂ�
	float growth = (posList[i] - posList[i - 1]).Len() - _naturalList[i - 1];
	//�΂˂̐L�т�͂ɕϊ�
	force += _springList[i - 1] * growth * (posList[i - 1] - posList[i]).Normalize();

	//����͔S����R 
	//��R�͂Ȃ̂Ł|�𑫂�
	force -= _viscousResistance * accelList[i];

	//�d�� 
	force += _massWeight * _gravity * _gravityDir;

	return force;
};

void bone::PositionReset() {
	//�����Ӂ@�ʒu�⑬�x�����������܂����A
	// �����ȃ��f���łȂ��Ə�����������d�͂̉e���œ����܂�
	for (int i = 0; i < _listSize; i++) {
		//���W�Ԋҍs��̏�����
		MV1ResetFrameUserLocalMatrix(*_model, _frameList[i + 1]);
	}

	for (int i = 0; i < _massPointSize; i++) {
		//�ʒu�̏�����
		_massPosList[i] = MV1GetFramePosition(*_model, _frameList[i + 1]);
		//���x���O�ɂ���
		_massAccelList[i].Set(0.0f, 0.0f, 0.0f);
	}
};