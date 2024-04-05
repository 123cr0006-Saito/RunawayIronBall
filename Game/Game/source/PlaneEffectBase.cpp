//----------------------------------------------------------------------
// @filename PlaneEffectBase.cpp
// ＠date: 2024/01/30
// ＠author: saito ko
// @explanation
// 板ポリゴンを表示するエフェクトの基底クラス
//----------------------------------------------------------------------
#include "PlaneEffectBase.h"
const unsigned short PlaneEffectBase::vertexOrder[6] = { 0,1,2,2,1,3 };
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param pos 位置
// @param sizeX x軸の横幅
// @param handle ハンドル
// @param handleMax ハンドルの最大数
// @param speed アニメーションの速度
//----------------------------------------------------------------------
PlaneEffectBase::PlaneEffectBase(VECTOR pos, int sizeX, int* handle, int handleMax, int speed) {
	_animCount = 0;
	_animMax = handleMax;
	_handle = NEW int[_animMax];
	for (int i = 0; i < _animMax; i++) {
	_handle[i] = handle[i];
	}
	_speed = speed;
	_currentTime = GetNowCount();
	//設定したサイズともともとの画像サイズから比率でサイズを割り出す
	int x, y;
	GetGraphSize(_handle[0], &x, &y);
	_sizeX = sizeX;
	_sizeY = y * _sizeX / x;
};

PlaneEffectBase::~PlaneEffectBase() {
	delete[] _handle;  _handle = nullptr;
};

bool PlaneEffectBase::Process(){
	if (GetNowCount() - _currentTime >= _speed) {
	_currentTime = GetNowCount();
	_animCount++;
	if (_animCount > _animMax) {
	_IsPlay = false;
	}
	}
	return true;
};

bool PlaneEffectBase::Render() {
	return true;
};

