//----------------------------------------------------------------------
// @filename EffectManeger.cpp
// ＠date: 2024/01/12
// ＠author: saito ko
// @explanation
// 全てのエフェクトを管理するためのクラス
//----------------------------------------------------------------------
#include "EffectManeger.h"
EffectManeger* EffectManeger::_instance = nullptr;
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return なし
//----------------------------------------------------------------------
EffectManeger::EffectManeger(){
	_instance = this;
	_effect.clear();
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return なし
//----------------------------------------------------------------------
EffectManeger::~EffectManeger() {
	_instance = nullptr;
	for (auto&& effect : _effect) {
		delete effect;
	}
	_effect.clear();
};
//----------------------------------------------------------------------
// @brief エフェクトを追加
// @return 無し
//----------------------------------------------------------------------
void EffectManeger::LoadEffect(EffectBase* effect) {
	_effect.push_back(effect);
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功したかどうか
//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
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