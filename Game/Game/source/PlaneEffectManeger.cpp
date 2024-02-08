#include "PlaneEffectManeger.h"
PlaneEffect::PlaneEffectManeger::PlaneEffectManeger(){

};

PlaneEffect::PlaneEffectManeger::~PlaneEffectManeger() {

};

void PlaneEffect::PlaneEffectManeger::LoadVertical(BoardPolygon* effect) {
	_vertical.push_back(effect);
};

bool PlaneEffect::PlaneEffectManeger::Update(){
	for (auto itr = _vertical.begin(); itr != _vertical.end(); ) {
		if ((*itr)->GetFlag()) {
			(*itr)->Update();
			++itr;
		}
		else {
			delete (*itr);
			itr = _vertical.erase(itr);
		}
	}
	return true;
};

bool PlaneEffect::PlaneEffectManeger::Render() {
	//�r���{�[�h�`�撆�̓��C�g�̌v�Z���ꎞ��~  ���C�g�Ɣ��Ε����������ƍ����Ȃ�
	SetUseLighting(FALSE);
	for (auto itr = _vertical.begin(); itr != _vertical.end(); ++itr) {
		(*itr)->Render();
	}
	SetUseLighting(TRUE);
	return true;
};