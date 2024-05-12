//----------------------------------------------------------------------
// @filename BoardPolygonDust.h
// @date: 2024/02/28
// @author: saito ko
// @explanation
// �����̔|���𐶐�����̃N���X
//----------------------------------------------------------------------
#pragma once
#include "BoardPolygon.h"
#include "PlaneEffectBase.h"

class BoardPolygonDust : public PlaneEffectBase
{
public:
	// X�̒�������ɉ摜�̑傫������Y�𓱂��o���܂��B
	BoardPolygonDust(VECTOR pos,int sizeX, int* handle, int handleMax,int animspeed ,int speed = 10);
	~BoardPolygonDust();
	bool Process()override;
	bool Render()override;
protected:
	static const int _dustMax = 10;// �����̍ő吔
	int _speed;// �����̃X�s�[�h
	VECTOR _dustPos[_dustMax];// �����̍��W
	VECTOR _standardPos;// �����̊���W
	VECTOR _vLength;// ����W����̍����̒���
};



