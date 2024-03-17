#pragma once
#include "EffectBase.h"
#include "EffekseerForDXLib.h"

//effekseer���g�����߂̌��ƂȂ�N���X
class EffekseerBase : public EffectBase
{
public:
	EffekseerBase(std::string name, VECTOR* pos, float size,float speed =1.0f , bool loopFlag = false,bool loadFlag = true);
	EffekseerBase(int handle, VECTOR* pos, float size, float speed = 1.0f, bool loopFlag = false, bool loadFlag = true);
	virtual ~EffekseerBase()override;
	virtual bool Process()override;
	virtual bool Render()override;

protected:
	int _effectResourceHandle;
	int _playingEffectHandle;
	VECTOR  _pos;
	float _speed;
	float _size;
	bool _loopFlag;
};

typedef EffekseerBase EffekseerFixed; //���W�Œ�̃G�t�F�N�g