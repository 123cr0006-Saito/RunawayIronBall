//----------------------------------------------------------------------
// @filename EffekseerRotation.h
// ＠date: 2024/03/07
// ＠author: saito ko
// @explanation
// 回転攻撃のエフェクトを再生するクラス
//----------------------------------------------------------------------
#pragma once
#include "EffekseerBase.h"
class EffekseerRotation :  public EffekseerBase
{
	typedef EffekseerBase base;
public:
	EffekseerRotation(int handle, VECTOR* pos, float size, VECTOR* rotation, float height = 0.0f, float speed = 1.0f, bool loopFlag = false);
	~EffekseerRotation()override;
	bool Process()override;
	bool Render()override;
	void SetLoop(bool Loop) { _loopFlag = Loop;}
protected: 
	static const int _maxEffect = 2;
	int _playingEffectHandle[_maxEffect];
	int _currentTime;
	VECTOR* _pos;
	VECTOR* _rotation;
};

