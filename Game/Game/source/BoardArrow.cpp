//----------------------------------------------------------------------
// @filename BoardArrow.cpp
// ��date: 2024/02/01
// ��author: saito ko
// @explanation
// �w��̕����ɖ���`�悷��N���X
//----------------------------------------------------------------------
#include "BoardArrow.h"
const unsigned short ::BoardArrow::vertexList[12] = {0,1,2,2,1,3,4,5,6,6,5,7};
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param name: �t�@�C����
// @param length: ���̒���
// @return �Ȃ�
//----------------------------------------------------------------------
BoardArrow::BoardArrow(std::string name,float length, int animMax):
	_handle(ResourceServer::LoadGraph("Arrow", name.c_str())),
	_animMax(animMax)
{
	// ������
	_animCount = 0;
	// uv���X�g
	float uvList[VERTEX_MAX][2] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},
		{0.0f,1.0f},
		{1.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f}
	};
	
	// ���̒���
	float Width = 50.0f;
	// �c�̒���
	_length = length;
	float posListX[VERTEX_MAX] = { -Width, Width, -Width, Width, -Width, Width, -Width, Width};
	float posListZ[VERTEX_MAX] = { 0.0f, 0.0f,_length, _length, _length, _length, _length, _length};
	// ���_�̐ݒ�
	for (int i = 0; i < VERTEX_MAX; i++) {
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
BoardArrow::~BoardArrow() {};
//----------------------------------------------------------------------
// @brief ���_�̍X�V
// @return �Ȃ�
//----------------------------------------------------------------------
void BoardArrow::UpdateVertex() {
	float Width = 50.0f;
	float ratio = static_cast<float>(_animCount) / _animMax;
	for (int i = 0; i < 2; i++) {
		VECTOR dirVec = _originPos[i+6];
		_originPos[i + 2].z = VScale(dirVec, ratio).z;
		_originPos[i + 4].z = VScale(dirVec, ratio).z;
		vertex[i].v = ratio;
		vertex[i+6].v = ratio;
	}
};
//----------------------------------------------------------------------
// @brief �X�V����
// @param pos: ���W
// @param dirY: y���̉�]�p�x
// @return �����������ǂ���
//----------------------------------------------------------------------
bool BoardArrow::Process(VECTOR pos, float dirY) {
	// �A�j���[�V������i�߂�
	_animCount++;
	if (_animCount > _animMax) {
		_animCount = 0;
	}
	// ���_�����X�V
	UpdateVertex();
	// y���s����g�p����]������
	MATRIX matrix = MGetRotY(dirY);
	for (int i = 0; i < VERTEX_MAX; i++) {
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
bool BoardArrow::Process(VECTOR pos, VECTOR dirVec) {
	// �����x�N�g������p�x���o���Ay���s��ŉ�]������
	float dirY = atan2(dirVec.x, dirVec.z);
	// �A�j���[�V������i�߂�
	_animCount++;
	if (_animCount > _animMax) {
		_animCount = 0;
	}
	// ���_�����X�V
	UpdateVertex();
	MATRIX matrix = MGetRotY(dirY);
	for (int i = 0; i < VERTEX_MAX; i++) {
		vertex[i].pos = VAdd(pos, VTransform(_originPos[i], matrix));
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool BoardArrow::Render() {
	SetUseBackCulling(false);
	DrawPolygonIndexed3D(vertex, VERTEX_MAX, vertexList, 4, _handle, true);
	SetUseBackCulling(true);
	return true;
};