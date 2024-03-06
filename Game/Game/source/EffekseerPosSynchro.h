#pragma once
#include "EffekseerBase.h"
//ç¿ïWÇ™ïœâªÇ∑ÇÈ effekseerÉNÉâÉX
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
	VECTOR* _pos;
	float _height;
};

