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
	//ビルボード描画中はライトの計算を一時停止  ライトと反対方向を向くと黒くなる
	SetUseLighting(FALSE);
	for (auto itr = _vertical.begin(); itr != _vertical.end(); ++itr) {
		(*itr)->Render();
	}
	SetUseLighting(TRUE);
	return true;
};