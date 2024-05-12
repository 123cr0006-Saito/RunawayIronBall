//----------------------------------------------------------------------
// @filename EffekseerRotation.h
// @date: 2024/03/07
// @author: saito ko
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
	static const int _maxEffect = 2;//エフェクトの表示最大数
	int _playingEffectHandle[_maxEffect];//エフェクトのハンドル
	int _currentTime;//エフェクトの再生時間
	VECTOR* _pos;// エフェクトの座標
	VECTOR* _rotation;// エフェクトの回転値
};

