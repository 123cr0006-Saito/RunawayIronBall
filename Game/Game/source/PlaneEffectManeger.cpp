#include "PlaneEffectManeger.h"

EffectManeger* EffectManeger::_instance = nullptr;

EffectManeger::EffectManeger(){
	_instance = this;
	_effect.clear();
};

EffectManeger::~EffectManeger() {

};

void EffectManeger::LoadEffect(EffectBase* effect) {
	_effect.push_back(effect);
};

bool EffectManeger::Update(){
	for (auto itr = _effect.begin(); itr != _effect.end(); ) {
		if ((*itr)->GetFlag()) {
			(*itr)->Process();
			++itr;
		}
		else {
			delete (*itr);
			itr = _effect.erase(itr);
		}
	}
	return true;
};

bool EffectManeger::Render() {
	//�r���{�[�h�`�撆�̓��C�g�̌v�Z���ꎞ��~  ���C�g�Ɣ��Ε����������ƍ����Ȃ�
	SetUseLighting(FALSE);
	for (auto itr = _effect.begin(); itr != _effect.end(); ++itr) {
		(*itr)->Render();
	}
	SetUseLighting(TRUE);
	return true;
};