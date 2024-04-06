//----------------------------------------------------------------------
// @filename Suppression.cpp
// ＠date: 2024/02/26
// ＠author: saito ko
// @explanation
// 敵や建物を破壊した時に減少していく制圧ゲージを管理するクラス
//----------------------------------------------------------------------
#include "Suppression.h"
Suppression* Suppression::_instance = nullptr;

Suppression::Suppression(){
	_instance = this;
	_maxSuppression = 0;
	_nowSuppression = 0;
};

Suppression::~Suppression() {
	_instance = nullptr;
};

void Suppression::AddSuppression(int suppression) { 
	_maxSuppression += suppression;
	_nowSuppression += suppression;
}

void Suppression::SubSuppression(int suppression) { if(_nowSuppression > 0)_nowSuppression -= suppression; }

void Suppression::ClearSuppression() {
	_maxSuppression = 0;
	_nowSuppression = 0;
};

bool Suppression::GetIsRatio() {
	// maxの20%以下ならtrue
	if (_nowSuppression <= static_cast<float>(_maxSuppression / 10 * (10 - 8))) {
		return true;
	}
	return false;
}