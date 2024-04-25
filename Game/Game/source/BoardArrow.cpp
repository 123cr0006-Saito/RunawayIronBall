//----------------------------------------------------------------------
// @filename BoardArrow.cpp
// ��date: 2024/02/01
// ��author: saito ko
// @explanation
// �w��̕����ɖ���`�悷��N���X
//----------------------------------------------------------------------
#include "BoardArrow.h"
const unsigned short ::BoardArrow::vertexList[6] = {0,1,2,2,1,3};
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
	float uvList[VERTEX_MAX][2] = {
		{0.0f,0.0f},
		{0.0f,1.0f},
		{1.0f,0.0f},
		{1.0f,1.0f}
	};
	
	// ���̒���
	float Width = 50.0f;
	// �c�̒���
	_length = length;
	float posListX[VERTEX_MAX] = { -Width, Width, -Width, Width};
	float posListZ[VERTEX_MAX] = { 0.0f, 0.0f,_length, _length};
	// ���_�̐ݒ�
	for (int i = 0; i < VERTEX_MAX; i++) {
		_originPos[i] = VGet(posListX[i], 0, posListZ[i]);
		vertex[i].norm = VGet(0,1,0);
		vertex[i].dif = GetColorU8(255, 0, 0, 255);
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
BoardArrow::~BoardArrow() {};
//----------------------------------------------------------------------
// @brief �X�V����
// @param pos: ���W
// @param dirY: y���̉�]�p�x
// @return �����������ǂ���
//----------------------------------------------------------------------
bool BoardArrow::Process(VECTOR pos, float dirY) {
	// ���_�̈ʒu��ݒ�
	for (int i = 0; i < VERTEX_MAX; i++) {
		MATRIX matrix = MGetTranslate(_originPos[i]);
		matrix = MMult(matrix, MGetScale(VScale(VGet(1, 1, 1), 1 + 0.25 * sin(2 * DX_PI * GetNowCount() / 2000))));
		matrix = MMult(matrix, MGetRotY(dirY));
		vertex[i].pos = VAdd(VAdd(pos, VGet(0, 100, 50)), VTransform(VGet(1, 1, 1), matrix));
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �X�V����
// @param pos: ���W
// @param dirVec: �����x�N�g��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool BoardArrow::Process(VECTOR pos, VECTOR dirVec) {
	// �����x�N�g������p�x���o���Ay���s��ŉ�]������
	float dirY = atan2(dirVec.x, dirVec.z);
	// ���_�̈ʒu��ݒ�
	for (int i = 0; i < VERTEX_MAX; i++) {
		MATRIX matrix = MGetTranslate(_originPos[i]);
		matrix = MMult(matrix, MGetScale(VScale(VGet(1, 1, 1), 1 + 0.25 * sin(2 * DX_PI * GetNowCount() / 2000))));
		matrix = MMult(matrix, MGetRotY(dirY));
		vertex[i].pos = VAdd(VAdd(pos,VGet(0,100,0)), VTransform(VGet(1, 1, 1), matrix));
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool BoardArrow::Render() {
	clsDx();
	SetUseBackCulling(false);
	SetUseLighting(false);
	DrawPolygonIndexed3D(vertex, VERTEX_MAX, vertexList, 4, _handle, true);
	SetUseLighting(true);
	SetUseBackCulling(true);
	return true;
};