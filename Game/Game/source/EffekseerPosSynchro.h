#pragma once
#include "EffekseerBase.h"
//À•W‚ª•Ï‰»‚·‚é effekseerƒNƒ‰ƒX
class EffekseerPosSynchro : public EffekseerBase
{
	typedef EffekseerBase base;
public :
	EffekseerPosSynchro(std::string name, VECTOR* pos, float size, float speed = 1.0f, bool loopFlag = false);
	EffekseerPosSynchro(int handle, VECTOR* pos, float size, float speed = 1.0f, bool loopFlag = false);
	~EffekseerPosSynchro()override;
	bool Process()override;
	bool Render()override;
private :
	VECTOR* _pos;
};

