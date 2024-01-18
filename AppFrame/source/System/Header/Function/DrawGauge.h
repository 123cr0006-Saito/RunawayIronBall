#pragma once
#include"dxlib.h"
#include "math.h"
#include "mymath.h"
#include "../../../Application/Global.h"
class DrawGauge
{
public:
	DrawGauge(int image,int start_pos,bool flag);
	DrawGauge(const char* name,int start_pos,bool flag);

	virtual bool Draw(VECTOR pos,float parcent,float parcent_max);//���̒l�ƍő�̒l

	static  const unsigned short _rightTraianglList[];//�E��]����n�߂��邽�߂̕`��̎��Ɏg���e���_�̃��X�g
	static  const unsigned short _leftTraianglList[];//����]����n�߂��邽�߂̕`��̎��Ɏg���e���_�̃��X�g
private:
	VERTEX2D vertex[10];//�`��̍ۂɎg���e���_
	int handle;//�Q�[�W�Ƃ��Ďg���摜�@�ǂݍ��݂̏ꍇ�̓��\�[�X�T�[�o�[���g�p
	int _startPosition;//�n�܂�ʒu�@�n�܂�ʒu�͂s�x�o�d
	int _transFlag;//���]�����邩

	//�J�n�n�_���
	enum TYPE : int {
		right = 0,
		down,
		left,
		up
	};
};

