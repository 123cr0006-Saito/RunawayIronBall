#pragma once
#include "appframe.h"
#include "EffekseerBase.h"
#include "EffekseerPosSynchro.h"
#include "PlaneEffectManeger.h"
#include "FrameData.h"
class ClassificationEffect
{
public:
	ClassificationEffect();
	~ClassificationEffect();
	void SetClassification(CommandParam param);
	static ClassificationEffect* _instance;
	static ClassificationEffect* GetInstance () { return _instance; }
protected:
	std::map<int, std::string> _commandList;
};

