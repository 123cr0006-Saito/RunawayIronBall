//----------------------------------------------------------------------
// @filename Gate.h
// ��date: 2024/03/15
// ��author: saito ko
// @explanation
// �v���C���[���G���΃N���A�ɂȂ�I�u�W�F�N�g�N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class Gate
{
public:
	Gate(VECTOR pos, int r, int* handle,int size, int animSpeed,int handlesize );
	~Gate();
	void Process();
	void Draw();
	VECTOR  GetPos() { return _pos; };
	float GetR() { return _r; };
protected:
	VECTOR _pos;//���W
	int* _handle;//�摜�n���h��
	int _r;//���a
	int _size;//�摜����
	int _handleSize;//�摜�̉���
    int _animCount;//�A�j���[�V�����J�E���g
	int _animSpeed;//�A�j���[�V�����̍Đ����x
	int _currentTime;//���݂̎���
};

