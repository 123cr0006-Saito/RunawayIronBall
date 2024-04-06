//----------------------------------------------------------------------
// @filename BoardArrow.cpp
// ��date: 2024/02/01
// ��author: saito ko
// @explanation
// �w��̕����ɖ���`�悷��N���X
//----------------------------------------------------------------------
#include "BoardArrow.h"
const unsigned short ::BoardArrow::vertexList[6] = { 0,1,2,2,1,3 };
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param name: �t�@�C����
// @param length: ���̒���
// @return �Ȃ�
//----------------------------------------------------------------------
BoardArrow::BoardArrow(std::string name,float length):
	_handle(ResourceServer::LoadGraph("Arrow", name.c_str()))
{
	// ������
	// uv���X�g
	float uvList[4][2] = {
		{0.0 ,0.0 },
		{1.0f,0.0 },
		{0.0 ,1.0f},
		{1.0f,1.0f}
	};
	
	// ���̒���
	float Width = 50.0f;
	// �c�̒���
	_length = length;
	float posListX[4] = { -Width,Width, -Width ,Width };
	float posListZ[4] = { _length,_length,0.0f,0.0f };
	// ���_�̐ݒ�
	for (int i = 0; i < 4; i++) {
		_originPos[i] = VGet(posListX[i], 50, posListZ[i]);
		vertex[i].norm = VGet(0,1,0);
		vertex[i].dif = GetColorU8(255, 255, 255, 255);
		vertex[i].spc = GetColorU8(0, 0, 0, 0);
		vertex[i].u = uvList[i][0];
		vertex[i].v = uvList[i][1];
		vertex[i].su = 0.0f;
		vertex[i].sv = 0.0f;
	}
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return �Ȃ�
//----------------------------------------------------------------------
BoardArrow::~BoardArrow() {

};
//----------------------------------------------------------------------
// @brief �X�V����
// @param pos: ���W
// @param dirY: y���̉�]�p�x
// @return �����������ǂ���
//----------------------------------------------------------------------
bool BoardArrow::Update(VECTOR pos, float dirY) {
	// y���s����g�p����]������
	MATRIX matrix = MGetRotY(dirY);
	for (int i = 0; i < 4; i++) {
		vertex[i].pos = VAdd(pos,VTransform(_originPos[i], matrix));
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �X�V����
// @param pos: ���W
// @param dirVec: �����x�N�g��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool BoardArrow::Update(VECTOR pos, VECTOR dirVec) {
	// �����x�N�g������p�x���o���Ay���s��ŉ�]������
	float dirY = atan2(dirVec.x, dirVec.z);
	MATRIX matrix = MGetRotY(dirY);
	for (int i = 0; i < 4; i++) {
		vertex[i].pos = VAdd(pos, VTransform(_originPos[i], matrix));
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool BoardArrow::Render() {
	DrawPolygonIndexed3D(vertex, VERTEX_MAX, vertexList,  2, _handle, true);
	return true;
};