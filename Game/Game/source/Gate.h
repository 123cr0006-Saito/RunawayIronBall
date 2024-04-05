//----------------------------------------------------------------------
// @filename Gate.h
// ＠date: 2024/03/15
// ＠author: saito ko
// @explanation
// プレイヤーが触れればクリアになるオブジェクトクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
class Gate
{
public:
	Gate(VECTOR pos, int r, int* handle,int size, int animSpeed,int handlesize );
	~Gate();
	void Process();
	void Draw();
	VECTOR  GetPos() { return _pos; };
	float GetR() { return _r; };
protected:
	VECTOR _pos;//座標
	int* _handle;//画像ハンドル
	int _r;//半径
	int _size;//画像枚数
	int _handleSize;//画像の横幅
    int _animCount;//アニメーションカウント
	int _animSpeed;//アニメーションの再生速度
	int _currentTime;//現在の時間
};

