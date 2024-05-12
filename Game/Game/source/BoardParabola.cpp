//----------------------------------------------------------------------
// @filename BoardParabola.cpp
// @date: 2024/02/01
// @author: saito ko
// @explanation
// �w��̕����ɕ������̖���`�悷��N���X
//----------------------------------------------------------------------
#include "BoardParabola.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param name : �t�@�C����
// @return �Ȃ�
//----------------------------------------------------------------------
BoardParabola::BoardParabola(std::string name) :
	_handle(ResourceServer::LoadGraph("Arrow",name.c_str()))
{
	// ���_���̏�����
	for (int i = 0; i < VERTEX_MAX * (ONE_ROW_POLYGON_MAX / 2) + 2; i++) {
		_vertex[i].dif = GetColorU8(200, 255, 255, 100);
		_vertex[i].norm = VGet(0.0f, 1.0f, 0.0f);
		_vertex[i].spc = GetColorU8(0, 0, 0, 0);
	}

	// ���_���̐ݒ�
	float uvPos = 0;
	for (int i = 0; i <= ONE_ROW_POLYGON_MAX; i++) {
		_vertex[i * 2].u = uvPos / ONE_ROW_POLYGON_MAX;
		_vertex[i * 2].v = 0;
		_vertex[i * 2 + 1].u = uvPos / ONE_ROW_POLYGON_MAX;
		_vertex[i * 2 + 1].v = 1.0f;
		uvPos++;
	}
	// ���_�C���f�b�N�X�̐ݒ�
	for (int i = 0; i < ONE_ROW_POLYGON_MAX; i++) {
		_vertexOrder[i * 6] = 0 + 2 * i;
		_vertexOrder[i * 6 + 1] = 2 + 2 * i;
		_vertexOrder[i * 6 + 2] = 1 + 2 * i;
		_vertexOrder[i * 6 + 3] = 1 + 2 * i;
		_vertexOrder[i * 6 + 4] = 2 + 2 * i;
		_vertexOrder[i * 6 + 5] = 3 + 2 * i;
	}
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return �Ȃ�
//----------------------------------------------------------------------
BoardParabola::~BoardParabola() {
};
//----------------------------------------------------------------------
// @brief �X�V����
// @param pos : �ʒu
// @param dirY : Y���̉�]
// @param length : +z���̒���
// @param inclination : �X��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool BoardParabola::Update(VECTOR pos, float dirY, float length, float inclination) {
	// �K�v�ȕϐ��̐錾
	MATRIX matrix = MGetRotY(dirY);
	float harfLength = length / 2;
	float Length = -harfLength;
	// ���_���̐ݒ�
	for (int i = 0; i <= ONE_ROW_POLYGON_MAX; i++) {
		// ���_�̍��W��ݒ�
		float depth = (Length + harfLength) * -1;
		_vertex[i * 2].pos.x = 50;
		_vertex[i * 2].pos.y = -inclination * pow(Length, 2) + inclination * pow(harfLength, 2);
		_vertex[i * 2].pos.z = depth;
		// ���_�̍��W���s��ŕϊ�
		_vertex[i * 2].pos = VTransform(_vertex[i * 2].pos, matrix);
		_vertex[i * 2].pos = VAdd(pos, _vertex[i * 2].pos);
		// ���_�̍��W��ݒ�
		_vertex[i * 2 + 1].pos.x = -50;
		_vertex[i * 2 + 1].pos.y = -inclination * pow(Length, 2) + inclination * pow(harfLength, 2);
		_vertex[i * 2 + 1].pos.z = depth;
		// ���_�̍��W���s��ŕϊ�
		_vertex[i * 2 + 1].pos = VTransform(_vertex[i * 2 + 1].pos, matrix);
		_vertex[i * 2 + 1].pos = VAdd(pos, _vertex[i * 2 + 1].pos);
		// �����𑝂₷
		Length += length / ONE_ROW_POLYGON_MAX;
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool BoardParabola::Render() {
	DrawPolygonIndexed3D(_vertex, VERTEX_MAX / 2 * ONE_ROW_POLYGON_MAX + 2, _vertexOrder, ONE_ROW_POLYGON_MAX * 2, _handle, true);
	return true;
};