//----------------------------------------------------------------------
// @filename EffekseerBase.h
// ＠date: 2024/02/8
// ＠author: saito ko
// @explanation
// エフェクシアを再生するための基本的なクラス
//----------------------------------------------------------------------
#pragma once
#include "EffectBase.h"
#include "EffekseerForDXLib.h"

//effekseerを使うための元となるクラス
class EffekseerBase : public EffectBase
{
public:
	EffekseerBase(std::string name, VECTOR* pos, float size, float height = 0.0f, float speed =1.0f , bool loopFlag = false,bool loadFlag = true);
	EffekseerBase(int handle, VECTOR* pos, float size, float height = 0.0f, float speed = 1.0f, bool loopFlag = false, bool loadFlag = true);
	virtual ~EffekseerBase()override;
	virtual bool Process()override;
	virtual bool Render()override;

protected:
	int _effectResourceHandle;
	int _playingEffectHandle;
	VECTOR  _pos;
	float _height;
	float _speed;
	float _size;
	bool _loopFlag;
};

typedef EffekseerBase EffekseerFixed; //座標固定のエフェクト