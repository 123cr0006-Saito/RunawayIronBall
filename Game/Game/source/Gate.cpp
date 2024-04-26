//----------------------------------------------------------------------
// @filename Gate.h
// @date: 2024/03/15
// @author: saito ko
// @explanation
// プレイヤーが触れればクリアになるオブジェクトクラス
//----------------------------------------------------------------------
#include "Gate.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param pos 位置
// @param r 半径
// @param handle 画像ハンドル
// @param size 画像ハンドルの最大数
// @param animSpeed アニメーションの再生速度
// @param handlesize ビルボードで表示するときの横幅
// @return 無し
//----------------------------------------------------------------------
Gate::Gate(VECTOR pos, int r, int* handle, int size, int animSpeed, int handlesize) :
_pos(pos),
_r(r),
_size(size),
_animSpeed(animSpeed),
_handleSize(handlesize)
{
	_handle = NEW int[_size];
	for (int i = 0; i < _size; i++) {
		_handle[i] = handle[i];
	}
	_animCount = 0;
	_currentTime = GetNowCount();
	_animCount = 0;
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
Gate::~Gate() {
	delete[] _handle; _handle = nullptr;
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 無し
//----------------------------------------------------------------------
void Gate::Process(){
	if (GetNowCount() - _currentTime >= _animSpeed) {
		_currentTime = GetNowCount();
		_animCount++;
		if (_animCount >= _size) {
			_animCount = 0;
		}
	}
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 無し
//----------------------------------------------------------------------
void Gate::Draw() {
	DrawBillboard3D(_pos, 0.5f, 0.5f, _handleSize, 0.0f, _handle[_animCount], true);
};