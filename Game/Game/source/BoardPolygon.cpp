//----------------------------------------------------------------------
// @filename BoardPolygon.cpp
// ��date: 2024/02/28
// ��author: saito ko
// @explanation
// �|���𐶐����邽�߂̃N���X
//----------------------------------------------------------------------
#include "BoardPolygon.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param pos: �|���S���̒��S�_
// @param dir: z��(����)�̕���
// @param sizeX: �����̒���
// @param handle: �摜�n���h��
// @param handleMax: �A�j���[�V�����̐؂�ւ���
// @param speed: �A�j���[�V�����X�s�[�h
// @return �Ȃ�
//----------------------------------------------------------------------
BoardPolygon::BoardPolygon(VECTOR pos, VECTOR dir, int sizeX, int* handle, int handleMax, int speed)
	: PlaneEffectBase(pos, sizeX, handle, handleMax, speed) {

	//�������W�n�����߂�
	VECTOR PolyVecX = VNorm(VCross(dir, VGet(0, -1, 0)));
	VECTOR PolyVecY = VNorm(VCross(dir, PolyVecX));

	VECTOR VecSize[4] = {
		VAdd(VAdd(VScale(PolyVecX,-sizeX / 2),VScale(PolyVecY, _sizeY / 2)),pos),
		VAdd(VAdd(VScale(PolyVecX, sizeX / 2),VScale(PolyVecY, _sizeY / 2)),pos),
		VAdd(VAdd(VScale(PolyVecX,-sizeX / 2),VScale(PolyVecY,-_sizeY / 2)),pos),
		VAdd(VAdd(VScale(PolyVecX, sizeX / 2),VScale(PolyVecY,-_sizeY / 2)),pos)
	};
	//uv���X�g
	float uvList[4][2] = {
	{0.0 ,0.0 },
	{1.0f,0.0 },
	{0.0 ,1.0f},
	{1.0f,1.0f}
	};
	//������
	for (int i = 0; i < 4; i++) {
		_vertex[i].pos = VecSize[i];
		//�������Ă�������̋t�Ȃ̂�*-1
		_vertex[i].norm = VScale(dir, -1);
		_vertex[i].dif = GetColorU8(255, 255, 255, 255);
		_vertex[i].spc = GetColorU8(0, 0, 0, 0);
		_vertex[i].u = uvList[i][0];
		_vertex[i].v = uvList[i][1];
		_vertex[i].su = 0.0f;
		_vertex[i].sv = 0.0f;
	}

};
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param pos: �|���S���̒��S�_
// @param matrix: ��]�s��
// @param sizeX: �����̒���
// @param handle: �摜�n���h��
// @param handleMax: �A�j���[�V�����̐؂�ւ���
// @param speed: �A�j���[�V�����X�s�[�h
// @return �Ȃ�
//----------------------------------------------------------------------
BoardPolygon::BoardPolygon(VECTOR pos, MATRIX matrix, int sizeX, int* handle, int handleMax, int speed)
	:PlaneEffectBase(pos, sizeX, handle, handleMax, speed) {

	// ���_�̍��W�����߂�
	VECTOR VecSize[4] = {
	VAdd(pos,VTransform(VGet(-sizeX / 2, _sizeY / 2,0), matrix)),
	VAdd(pos,VTransform(VGet(sizeX / 2,  _sizeY / 2,0), matrix)),
	VAdd(pos,VTransform(VGet(-sizeX / 2,-_sizeY / 2,0), matrix)),
	VAdd(pos,VTransform(VGet(sizeX / 2, -_sizeY / 2,0), matrix))
	};
	//uv���X�g
	float uvList[4][2] = {
		{0.0 ,0.0 },
		{1.0f,0.0 },
		{0.0 ,1.0f},
		{1.0f,1.0f}
	};

	//�s�񂩂�t�H���[�h�x�N�g�������o��
	VECTOR norm = Math::MatrixToVector(matrix, 2);
	//������
	for (int i = 0; i < 4; i++) {
		_vertex[i].pos = VecSize[i];
		_vertex[i].norm = norm;
		_vertex[i].dif = GetColorU8(255, 255, 255, 255);
		_vertex[i].spc = GetColorU8(0, 0, 0, 0);
		_vertex[i].u = uvList[i][0];
		_vertex[i].v = uvList[i][1];
		_vertex[i].su = 0.0f;
		_vertex[i].sv = 0.0f;
	}

};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return �Ȃ�
//----------------------------------------------------------------------
BoardPolygon::~BoardPolygon() {
	PlaneEffectBase::~PlaneEffectBase();
};
//----------------------------------------------------------------------
// @brief ���C������
// @return �����������ǂ���
//----------------------------------------------------------------------
bool BoardPolygon::Process() {
	PlaneEffectBase::Process();
	return true;
};
//----------------------------------------------------------------------
// @brief �`��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool BoardPolygon::Render() {
	if (_IsPlay) {
		DrawPolygonIndexed3D(_vertex, 4, vertexOrder, 2, _handle[_animCount], true);
	}
	return true;
};
