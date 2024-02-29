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