//----------------------------------------------------------------------
// @filename EffectBase.cpp
// ��date: 2024/01/12
// ��author: saito ko
// @explanation
// �G�t�F�N�g���Đ����邽�߂̊��N���X
//----------------------------------------------------------------------
#include "EffectBase.h"
EffectBase::EffectBase(){
	_IsPlay = true;
};

EffectBase::~EffectBase() {

};

bool EffectBase::Process(){
	return true;
};

bool EffectBase::Render() {
	return true;
};