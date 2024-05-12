//----------------------------------------------------------------------
// @filename EffectManeger.h
// @date: 2024/01/12
// @author: saito ko
// @explanation
// �S�ẴG�t�F�N�g���Ǘ����邽�߂̃N���X
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
	std::vector<EffectBase*> _effect;//�G�t�F�N�g���i�[���邽�߂̕ϐ�
};
