#pragma once
#include <vector>
#include "dxlib.h"
#include "appframe.h"
class bone
{
public:
	bone(int* Model, std::vector<int> list, int size);
	~bone();
	void SetMain(Vector3D* pos_list);//bone�̈ʒu���Z�b�g����
	void SetBoneDir(Vector3D world_dir_vec, Vector3D boon_pos, int target_frame, int parent_frame, MATRIX trans_mat, Vector3D dir_parent);;
	static const Vector3D _orign;

	Vector3D* _orignPos;

	Vector3D GetOrignPos(int index) { return _orignPos[index]; }//�����n�_�̍��W�𓾂�
private:
	int* _model;
	int _listSize;
	Vector3D* _vecDirList;//�������_�ł̃{�[�����猩��e�̕���
	MATRIX* _transMatrixList;//���W�ϊ��s��̃��X�g
	std::vector<int> _frameList;//�t���[���̔ԍ����X�g

	// �� �������牺�͕������Z�Ŏg���ϐ���֐�
public:
	Vector3D* _massPosList; // ���_�̍��W���X�g
	Vector3D* _massAccelList; // ���_�̑��x���X�g

	void UpdatePosAndAccel(double _elapsedTime);
	Vector3D ForceWorksToMassPoint(int i, Vector3D* posList, Vector3D* accelList); //���_�ɓ����͂��v�Z F=ma
	bool Process();

	void PositionReset();

private:

	// ����͔��̖т����Ȃ̂Œ萔�錾���Ă��邪�ق��̂Ɏg���Ƃ���
	//�ϐ��Ƃ��Đ錾���d�͂Ȃǂ̒l���N���X�����Ƃ��̈����Ƃ��ē��͂���Ƃł���
	//�����������̂ŕʃN���X�ō�����ق����ǂ����������H
	static const float _massWeight;//���_�̏d��
	static const float _viscousResistance;//�S����R
	static const float _gravity; //�d��
	static const float _spring; //�΂˒萔
	static const double _processInterval; //�����̍ו����̊Ԋu

	static const float _naturalCorrectionFactor; //���̖т̎��R�̂̒������o���Ƃ��Ɏg�p����␳�W��
	static const Vector3D _gravityDir;//�d�͂̕��� 

	int _massPointSize; //���_��
	std::vector<float> _springList; // �΂˒萔�̃��X�g
	std::vector<float> _naturalList; // �΂˂̒����̃��X�g
};

