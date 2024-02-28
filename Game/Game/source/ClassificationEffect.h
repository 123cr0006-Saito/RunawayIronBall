#pragma once
#include "appframe.h"
#include "EffekseerBase.h"
#include "EffekseerPosSynchro.h"
#include "PlaneEffectManeger.h"
#include "FrameData.h"

namespace {
	constexpr int Play_Vibration_X = 1000;
	constexpr int Play_Vibration_Y = 1001;
	constexpr int Play_SE = 1002;
	constexpr int Play_CameraVibration = 1003;
	constexpr int Play_Effect_PC = 1004;
	constexpr int Play_Effect_IC = 1005;
	constexpr int Play_Effect_IU = 1006;
	constexpr int Play_Effekseer_PC = 1007;
	constexpr int Play_Effekseer_IC = 1008;
	constexpr int Play_Effekseer_IU = 1009;
}

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

