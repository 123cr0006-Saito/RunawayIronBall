#pragma once
#include <string>
#include "appframe.h"
#include "EffekseerForDXLib.h"

//effekseer���g�����߂̌��ƂȂ�N���X
class EffekseerBase
{
public:
	EffekseerBase(std::string name, VECTOR* pos, float size,float speed =1.0f , bool loopFlag = false);
	EffekseerBase(int handle, VECTOR* pos, float size, float speed = 1.0f, bool loopFlag = false);
	~EffekseerBase();

	virtual bool Process();
	static bool Render();

	static void processOnce();

	bool GetFlag() { return _useFlag; }
protected:
	int _effectResourceHandle;
	int _playingEffectHandle;
	VECTOR  _pos;
	bool _useFlag;
	float _speed;
	float _size;
	bool _loopFlag;
};

typedef EffekseerBase EffekseerFixed; //���W�Œ�̃G�t�F�N�g