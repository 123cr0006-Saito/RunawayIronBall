//----------------------------------------------------------------------
// @filename UIBase.cpp
// ＠date: 2023/12/19
// ＠author: saito ko
// @explanation
// UIの基底クラス
//----------------------------------------------------------------------
#include "UIBase.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param pos 位置
// @return 無し
//----------------------------------------------------------------------
UIBase::UIBase(VECTOR pos) {
	_pos = pos;
	_handleNum = 0;
	_cx = 0;
	_cy = 0;
};
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param pos 位置
// @param size ハンドルの数
// @param handle ハンドル
UIBase::UIBase(VECTOR pos, int size, int* handle) {
	_handle = NEW int[size];
	for (int i = 0; i < size; i++) {
		_handle[i] = handle[i];
	}
	_pos = pos;
	_handleNum = 0;
	float _x, _y;
	GetGraphSizeF(_handle[_handleNum], &_x, &_y);
	_cx = _x / 2;
	_cy = _y / 2;
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
UIBase::~UIBase() {
	if (_handle != nullptr) {
		delete[] _handle; _handle = nullptr;
	}
};
//----------------------------------------------------------------------
// @brief 更新処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool UIBase::Process() {
	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool UIBase::Draw() {
	float _cx, _cy;
	GetGraphSizeF(_handle[0], &_cx, &_cy);
	
	_cx = _cx /= 2;
	_cy = _cy /= 2;

	DrawGraphF(_pos.x - _cx, _pos.y - _cy, _handle[_handleNum], true);
	return true;
};