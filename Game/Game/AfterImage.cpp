//----------------------------------------------------------------------
// @filename AfterImage.cpp
// ��date: 2024/04/26
// ��author: Morozumi Hiroya
// @explanation
// �L�����N�^�[�̎c���\�����s���N���X
// �������ɂ������f���𕡐��̕\�����邱�ƂŎc����\������
//----------------------------------------------------------------------
#include "AfterImage.h"

AfterImage::AfterImage()
{
}

AfterImage::~AfterImage()
{
	for(auto itr = _modelInfo.begin(); itr != _modelInfo.end(); ++itr)
	{
		delete (*itr);
		(*itr) = nullptr;
	}
	_modelInfo.clear();
}


