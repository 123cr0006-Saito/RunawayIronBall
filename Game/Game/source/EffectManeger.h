//----------------------------------------------------------------------
// @filename EffectManeger.h
// @date: 2024/01/12
// @author: saito ko
// @explanation
// 全てのエフェクトを管理するためのクラス
//----------------------------------------------------------------------
#pragma once
#include "EffectBase.h"
#include "EffekseerForDXLib.h"
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
	std::vector<EffectBase*> _effect;//エフェクトを格納するための変数
};
