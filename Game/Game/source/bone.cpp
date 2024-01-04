#include "bone.h"

const VECTOR bone::orign = VGet(0, 0, 0);

const float bone::_massWeight = 0.2f;
const float bone::_viscousResistance = 1.5f;
const float bone::_gravity = 400.0f;
const float bone::_spring = 50.0f;
const float bone::_naturalCorrectionFactor = 1.0f;

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
	model(Model),
	Frame_list(list),
	list_size(size),
	_springList(size + 1),
	_naturalList(size + 1)
{
	//�{�[���̏�����
	vec_dir_list = new VECTOR[list_size];
	orign_pos = new VECTOR[list_size + 2];
	trans_mat_list = new MATRIX[list_size];


	for (int i = 0; i < list_size; i++) {
		trans_mat_list[i] = MV1GetFrameLocalMatrix(*model, Frame_list[i + 1]);
		MATRIX local_mat = MV1GetFrameLocalMatrix(*model, Frame_list[i + 2]);
		vec_dir_list[i] = VTransform(orign, local_mat);

		for (int j = 0; j < 3; j++) {
			trans_mat_list[i].m[3][j] = 0.0f;
		}
		orign_pos[i + 2] = MV1GetFramePosition(*model, Frame_list[i + 2]);
	}
	orign_pos[0] = MV1GetFramePosition(*model, Frame_list[0]);
	orign_pos[1] = MV1GetFramePosition(*model, Frame_list[1]);

	//----------------------------------------------------------------------------------
	//�������Z�����邽�߂̕ϐ��̏�����
	_massPointSize = Frame_list.size() - 1;
	_massPosList = new Vector3D[_massPointSize];
	_massAccelList = new Vector3D[_massPointSize];

	for (int i = 0; i < _massPointSize; i++) {
		_massPosList[i].Set(MV1GetFramePosition(*model, Frame_list[i + 1]));
	}

	for (int i = 0; i < _massPointSize - 1; i++) {
		//_naturalList�̏�����
		//_natulalCorrectionFactor�͒ʏ�1.0�ŗǂ��Ǝv�����A
		// ���̖т��ُ�ɒ����Ȃ����Ƃ���_natulalCorrectionFactor��ύX��
		// ���̃��f���̔��̒����ɋ߂���Ԃɂ���
		_naturalList[i] = _naturalCorrectionFactor * (_massPosList[i + 1] - _massPosList[i]).len();

		//_springList�̏�����
		//�΂˂̒l�͌��܂��Ă��邪_naturalCorrectionFactor�ɔ�Ⴕ�ĕύX����
		//����͎��_�����̒��������K�v�Ȃ���
		// ���_�ɍ��킵�Ă΂˒萔���ύX�\
		_springList[i] = _naturalList[i] * _spring;
	}
};

bone::~bone() {
	SAFE_DELETE(vec_dir_list);
	SAFE_DELETE(orign_pos);
	SAFE_DELETE(trans_mat_list);
	model = nullptr;
};

void bone::SetMain(Vector3D* pos_list) {
	for (int i = 0; i < list_size; i++) {
		SetBoneDir(
			pos_list[i + 1].toVECTOR(),
			pos_list[i].toVECTOR(),
			Frame_list[i + 1],
			Frame_list[i],
			trans_mat_list[i],
			vec_dir_list[i]
		);
	};
};

void bone::SetBoneDir(
	VECTOR world_dir_vec,
	VECTOR boon_pos,
	int target_frame,
	int parent_frame,
	MATRIX trans_mat,
	VECTOR dir_parent)
{
	MATRIX TmpMat;
	TmpMat = MV1GetFrameLocalWorldMatrix(*model, parent_frame); // �e�{�[���̃��[�J�������[���h
	TmpMat = MInverse(TmpMat); // �t�s����v�Z
	VECTOR Local_Dir_Vec = VTransform(world_dir_vec, TmpMat);//�{�[��������̃��[�J�����W
	VECTOR Local_Bone_Pos = VTransform(boon_pos, TmpMat);//�����̕t�����̃��[�J�����W

	//�{�[����������������
	VECTOR Local_Dir_Look = VNorm(VSub(Local_Dir_Vec, Local_Bone_Pos));

	//���̌������玟�̌����ւ̉�]�s��
	MATRIX RotMat = MGetRotVec2(dir_parent, Local_Dir_Look);

	//�����̕t�������ʒu�ɂȂ�悤�A���s�ړ�
	MATRIX shift_point = trans_mat;
	shift_point.m[3][0] = Local_Bone_Pos.x;
	shift_point.m[3][1] = Local_Bone_Pos.y;
	shift_point.m[3][2] = Local_Bone_Pos.z;

	TmpMat = MMult(RotMat, shift_point);
	MV1SetFrameUserLocalMatrix(*model, target_frame, TmpMat);
}

bool bone::Process() {

	double _elapsedTime = global._timer->GetElapsedTime();
	//int processCount = 0;
	while (1)
	{
		if (_elapsedTime < _processInterval)break;
		_elapsedTime -= _processInterval;
		UpdatePosAndAccel(_processInterval);
		//processCount++;
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
	Vector3D* newPosList = new Vector3D[_massPointSize];
	Vector3D* newAccelList = new Vector3D[_massPointSize];

	//�t�����̈ʒu�͌Œ�
	_massPosList[0] = MV1GetFramePosition(*model, Frame_list[1]);
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
	Vector3D _force;

	//�s������
	//�΂˂̂荇�� ������̒e���͂Ɖ������̏d�͂��荇�����v�Z
	//��R������
	//�d�͂�������

	//���_i�`���_i+1�Ԃ̂΂˂���󂯂��
	if (i < _massPointSize - 1) {
		//�΂˂̐L�ы�𒲂ׂ�
		float _growth = (posList[i + 1] - posList[i]).len() - _naturalList[i];
		//�΂˂̐L�т�͂ɕϊ�
		_force += _springList[i] * _growth * (posList[i + 1] - posList[i]).normalize();
	}

	// ���_i-1�`���_i�Ԃ̃o�l����󂯂�� 
	//�΂˂̐L�ы�𒲂ׂ�
	float _growth = (posList[i] - posList[i - 1]).len() - _naturalList[i - 1];
	//�΂˂̐L�т�͂ɕϊ�
	_force += _springList[i - 1] * _growth * (posList[i - 1] - posList[i]).normalize();

	//����͔S����R 
	//��R�͂Ȃ̂Ł|�𑫂�
	_force -= _viscousResistance * accelList[i];

	//�d�� 
	_force += _massWeight * _gravity * _gravityDir;

	return _force;
};