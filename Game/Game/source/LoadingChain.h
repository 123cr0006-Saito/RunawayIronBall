//----------------------------------------------------------------------
// @filename LoadingChain.h
// ＠date: 2024/03/03
// ＠author: saito ko
// @explanation
// loading時の鎖を制御・描画を行うクラス
//----------------------------------------------------------------------
#pragma once
#include "dxlib.h"
#include "ModelColor.h"
#include "../AppFrame/source/System/Header/Function/mymath.h"
class LoadingChain
{
public:
	LoadingChain();
	~LoadingChain();
	void Init();
	void Process();
	void AnimProcess();
	void SetPlayerModelHandle(int handle);
	void Render();

	void SetBallPosition(VECTOR pos) { _iPos = pos; }

protected:
	// 鎖の個数
	static const int Chain_Num = 5;
	//鎖
	int _cModelHandle;
	VECTOR _cPos[Chain_Num];

	// 鉄球
	int _iModelHandle;
	VECTOR _iPos;
	VECTOR _ibDefaultScale;
	int _attackAnimCnt;
	int _animIndex;
	float _animTotalTime;
	float _playTime;

	// その他
	int _playerModelHandle;
	float _length;

	// 配置ソケット
	int _socketNo[3];

	ModelColor* _modelColor; // 2つのモデルの色を変更するが元に戻さないためインスタンスは一つにする
};

