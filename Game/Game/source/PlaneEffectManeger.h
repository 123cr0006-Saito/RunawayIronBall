#pragma once
#include "EffectBase.h"
#include <vector>
class EffectManeger
{
public:
	EffectManeger();
	~EffectManeger();

	void LoadEffect(EffectBase* effect);
	virtual bool Update();
	virtual bool Render();
	static EffectManeger* _instance;
	static EffectManeger* GetInstance() { return _instance; };
protected:
	std::vector<EffectBase*> _effect;
};
