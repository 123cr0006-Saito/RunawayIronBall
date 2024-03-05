#pragma once
#include "EffekseerBase.h"
class EffekseerRotation :  public EffekseerBase
{
	typedef EffekseerBase base;
public:
	EffekseerRotation(std::string name, VECTOR* pos, float size, VECTOR* rotation,float speed = 1.0f, bool loopFlag = false);
	EffekseerRotation(int handle, VECTOR* pos, float size, VECTOR* rotation, float speed = 1.0f, bool loopFlag = false);
	~EffekseerRotation()override;
	bool Process()override;
	bool Render()override;
	void SetLoop(bool Loop) { _loopFlag = Loop;}
protected: 
	int _playingEffectHandle[5];
	VECTOR* _pos;
	VECTOR* _rotation;
};

