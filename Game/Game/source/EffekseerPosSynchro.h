//----------------------------------------------------------------------
// @filename EffekseerPosSynchro.h
// @date: 2024/02/8
// @author: saito ko
// @explanation
// 位置情報のあるアドレスを受け取り移動するエフェクシアを再生するクラス
//----------------------------------------------------------------------
#pragma once
#include "EffekseerBase.h"
//座標が変化する effekseerクラス
class EffekseerPosSynchro : public EffekseerBase
{
	typedef EffekseerBase base;
public :
	EffekseerPosSynchro(std::string name, VECTOR* pos, float size, VECTOR rotation = VGet(0, 0, 0), float height = 0.0f, float speed = 1.0f, bool loopFlag = false);
	EffekseerPosSynchro(int handle, VECTOR* pos, float size, VECTOR rotation = VGet(0,0,0), float height = 0.0f, float speed = 1.0f, bool loopFlag = false);
	~EffekseerPosSynchro()override;
	bool Process()override;
	bool Render()override;
private :
	VECTOR* _pos;//座標のアドレス
	int _currentTime;//再生時間
};

