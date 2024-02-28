#pragma once
#include "EffectBase.h"
#include "EffekseerForDXLib.h"

//effekseerを使うための元となるクラス
class EffekseerBase : public EffectBase
{
public:
	EffekseerBase(std::string name, VECTOR* pos, float size,float speed =1.0f , bool loopFlag = false);
	virtual ~EffekseerBase()override;
	bool Process()override;

protected:
	int _effectResourceHandle;
	int _playingEffectHandle;
	VECTOR  _pos;
	float _speed;
	float _size;
	bool _loopFlag;
};

typedef EffekseerBase EffekseerFixed; //座標固定のエフェクト