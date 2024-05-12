//----------------------------------------------------------------------
// @filename BoardPolygonDust.cpp
// @date: 2024/02/28
// @author: saito ko
// @explanation
// �����̔|���𐶐�����̃N���X
//----------------------------------------------------------------------
#include "BoardPolygonDust.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param pos �����ʒu
// @param sizeX �T�C�Y
// @param handle �n���h��
// @param handleMax �n���h����
// @param speed ���x
// @param animspeed �A�j���[�V�����X�s�[�h
// @return �Ȃ�
//----------------------------------------------------------------------
BoardPolygonDust::BoardPolygonDust(VECTOR pos, int sizeX, int* handle, int handleMax, int speed,int animspeed) 
: PlaneEffectBase(pos,  sizeX, handle, handleMax, animspeed) {
	_standardPos = pos;
	_speed = speed;
	VECTOR oPos = VGet(0, 0, 0);
	_vLength = oPos;
	for (int i = 0; i < _dustMax; i++){
		_dustPos[i] = oPos;
	}
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return �Ȃ�
//----------------------------------------------------------------------
BoardPolygonDust::~BoardPolygonDust() {
	PlaneEffectBase::~PlaneEffectBase();
};
//----------------------------------------------------------------------
// @brief ���C������
// @return �����������ǂ���
//----------------------------------------------------------------------
bool BoardPolygonDust::Process() {
	PlaneEffectBase::Process();
	// ���x�����Z
	_vLength.y += static_cast<float>(_speed) / 2;
	_vLength.z += static_cast<float>(_speed);
	// �s��𐶐�
	MATRIX matrix = MGetTranslate(_vLength);
	VECTOR originPos = VGet(1, 1, 1);
	// �����ʒu���v�Z
	for (int i = 0; i < _dustMax; i++) {
		float rotation = 360.0f / _dustMax * i * DX_PI / 180.0f;
		matrix = MMult(matrix, MGetRotY(rotation));
		VECTOR vec = VTransform(originPos, matrix);
		_dustPos[i] = VAdd(_standardPos, vec);
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool BoardPolygonDust::Render() {
	for (int i = 0; i < _dustMax; i++) {
		DrawBillboard3D(_dustPos[i],0.5f,0.3f,_sizeX,0.0f, _handle[_animCount],true);
	}
	return true;
};
