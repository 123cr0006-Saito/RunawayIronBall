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
	int _effectResourceHandle;//エフェクトのリソースハンドル
	int _playingEffectHandle;//再生中のエフェクトハンドル
	VECTOR  _pos;//エフェクトの座標
	float _height;//エフェクトの高さ
	float _speed;//エフェクトの再生速度
	float _size;//エフェクトの大きさ
	bool _loopFlag;//エフェクトのループフラグ
};

typedef EffekseerBase EffekseerFixed; //座標固定のエフェクト