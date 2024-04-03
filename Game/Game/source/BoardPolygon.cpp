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
		polygon[i].pos = VecSize[i];
		//�������Ă�������̋t�Ȃ̂�*-1
		polygon[i].norm = VScale(dir, -1);
		polygon[i].dif = GetColorU8(255, 255, 255, 255);
		polygon[i].spc = GetColorU8(0, 0, 0, 0);
		polygon[i].u = uvList[i][0];
		polygon[i].v = uvList[i][1];
		polygon[i].su = 0.0f;
		polygon[i].sv = 0.0f;
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
//----------------------------------------------------------------------
BoardPolygon::BoardPolygon(VECTOR pos, MATRIX matrix, int sizeX, int* handle, int handleMax, int speed)
	:PlaneEffectBase(pos, sizeX, handle, handleMax, speed) {

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

	//�}�g���b�N�X����t�H���[�h�x�N�g�������o��
	VECTOR norm = Math::MatrixToVector(matrix, 2);
	//������
	for (int i = 0; i < 4; i++) {
		polygon[i].pos = VecSize[i];
		polygon[i].norm = norm;
		polygon[i].dif = GetColorU8(255, 255, 255, 255);
		polygon[i].spc = GetColorU8(0, 0, 0, 0);
		polygon[i].u = uvList[i][0];
		polygon[i].v = uvList[i][1];
		polygon[i].su = 0.0f;
		polygon[i].sv = 0.0f;
	}

};

BoardPolygon::~BoardPolygon() {
	PlaneEffectBase::~PlaneEffectBase();
};

bool BoardPolygon::Process() {
	PlaneEffectBase::Process();
	return true;
};

bool BoardPolygon::Render() {
	if (_IsPlay) {
		DrawPolygonIndexed3D(polygon, 4, vertexOrder, 2, _handle[_animCount], true);
	}
	return true;
};
