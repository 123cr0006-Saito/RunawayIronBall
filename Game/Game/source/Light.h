//----------------------------------------------------------------------
// @filename Light.h
// @date: 2024/03/01
// @author: saito ko
// @explanation
// �t�@�C������f�[�^��ǂݍ��݃��C�g��ݒ肷��N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class Light
{
public :
	Light(std::string fileName);
	~Light();
	bool LoadLightColor(std::string fileName);
protected:
	int _lightHandle;//���C�g�n���h��
};

