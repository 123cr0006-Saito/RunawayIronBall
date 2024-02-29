#include "EffectManeger.h"

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
	UpdateEffekseer3D();
	return true;
};

bool EffectManeger::Render() {
	//ビルボード描画中はライトの計算を一時停止  ライトと反対方向を向くと黒くなる
	SetUseLighting(FALSE);
	for (auto itr = _effect.begin(); itr != _effect.end(); ++itr) {
		(*itr)->Render();
	}
	SetUseLighting(TRUE);

	// エフェクシアの描画
	Effekseer_Sync3DSetting();
	DrawEffekseer3D();

	return true;
};