//----------------------------------------------------------------------
// @filename BoardArrow.h
// ��date: 2024/02/01
// ��author: saito ko
// @explanation
// �w��̕����ɖ���`�悷��N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include <string>
class BoardArrow
{
public:
	BoardArrow(std::string name, float length,int animMax);
	~BoardArrow();
	void UpdateVertex();
	bool Process(VECTOR pos, float dirY); //�����p�x
	bool Process(VECTOR pos, VECTOR dirVec); // �����x�N�g��
	bool Render();
protected:
	int _handle;// �摜�n���h��
	float _length;// ���̒���
	static const int VERTEX_MAX = 8;// ���_��
	static const unsigned short vertexList[12];// ���_���X�g
	VERTEX3D vertex[VERTEX_MAX];// ���_���
	VECTOR _originPos[VERTEX_MAX];// ���̌��_
	int _animCount; //�A�j���[�V�����̎���
	int _animMax;//�A�j���[�V�����̍ő厞��
};