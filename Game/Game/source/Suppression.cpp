//----------------------------------------------------------------------
// @filename Suppression.cpp
// @date: 2024/02/26
// @author: saito ko
// @explanation
// 敵や建物を破壊した時に減少していく制圧ゲージを管理するクラス
//----------------------------------------------------------------------
#include "Suppression.h"
Suppression* Suppression::_instance = nullptr;
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return 無し
//----------------------------------------------------------------------
Suppression::Suppression(){
	_instance = this;
	_maxSuppression = 0;
	_nowSuppression = 0;
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
Suppression::~Suppression() {
	_instance = nullptr;
};
//----------------------------------------------------------------------
// @brief 制圧値を追加
// @param suppression 追加する制圧値
// @return 無し
//----------------------------------------------------------------------
void Suppression::AddSuppression(int suppression) { 
	_maxSuppression += suppression;
	_nowSuppression += suppression;
}
//----------------------------------------------------------------------
// @brief 制圧値を減少
// @param suppression 減少する制圧値
// @return 無し
//----------------------------------------------------------------------
void Suppression::SubSuppression(int suppression) { if(_nowSuppression > 0)_nowSuppression -= suppression; }
//----------------------------------------------------------------------
// @brief 制圧値を削除
// @return 制圧値
//----------------------------------------------------------------------
void Suppression::ClearSuppression() {
	_maxSuppression = 0;
	_nowSuppression = 0;
};
//----------------------------------------------------------------------
// @brief制圧値の割合から20%以下かどうかを返す
// @return 20%以下か
//----------------------------------------------------------------------
bool Suppression::GetIsRatioBut20PercentLess() {
	// maxの20%以下ならtrue
	if (_nowSuppression <= static_cast<float>(_maxSuppression / 10 * (10 - 8))) {
		return true;
	}
	return false;
}