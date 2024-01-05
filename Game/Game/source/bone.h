#pragma once
#include <vector>
#include "dxlib.h"
#include "Vector3D.h"
#include "appframe.h"
class bone
{
public:
	bone(int* Model, std::vector<int> list, int size);
	~bone();
	void SetMain(Vector3D* pos_list);
	void SetBoneDir(VECTOR world_dir_vec, VECTOR boon_pos, int target_frame, int parent_frame, MATRIX trans_mat, VECTOR dir_parent);;
	static const VECTOR _orign;

	VECTOR* _orignPos;

	VECTOR GetOrignPos(int index) { return _orignPos[index]; }
private:
	int* _model;
	int _listSize;
	VECTOR* _vecDirList;
	MATRIX* _transMatrixList;
	std::vector<int> _frameList;

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

