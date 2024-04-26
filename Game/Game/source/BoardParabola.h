//----------------------------------------------------------------------
// @filename BoardParabola.h
// @date: 2024/02/01
// @author: saito ko
// @explanation
// �w��̕����ɕ������̖���`�悷��N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include <string>
class BoardParabola
{
public:
	BoardParabola(std::string name);
	~BoardParabola();
	bool Update(VECTOR pos, float dirY, float length, float inclination); // ������
	bool Render();
protected:
	int _handle;// �摜�n���h��
	static const int ONE_ROW_POLYGON_MAX = 100;//�|���S���̍ő吔
	static const int VERTEX_MAX = 4;//���_�̍ő吔
	unsigned short _vertexOrder[ONE_ROW_POLYGON_MAX * 6];// ���_�̏���
	VERTEX3D _vertex[ONE_ROW_POLYGON_MAX / 2 * VERTEX_MAX + 2];// ���_���
};

