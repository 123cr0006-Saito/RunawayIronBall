#pragma once
//Enemy�N���X���Ǘ�����Pool��Enemy�{�̂ł���Base�œ����\���̂��g�p���邽�߂ɍ�����w�b�_�[�ł��B
//���̕��@���v�������炱�̃w�b�_�[���폜����Ǝv���܂��B

typedef struct {
	//�G���ʗp�̖��O
	int _hp;//�G�̗̑�
	int _exp;//�G���瓾����o���l
	float  _speed;//�ړ����x
	int _coolTime;//�U����̃N�[���^�C��
	//���G�n�ϐ�
	float _flontAngle;//���E�͈͂̊p�x
	float _hearingRangeSize;//���o�͈͂̔��a
	float _moveRange;//�ړ��͈͂̔��a
	float _sartchRange;//���G�͈͂̔��a
	float _discoverRangeSize;//�������A�Ώۂ̌������܂ł̋����̔��a
	float _attackRangeSize;//���ʂ̍U���͈�
}EnemyParam;