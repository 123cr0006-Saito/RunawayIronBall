#pragma once
#include "appframe.h"
#include "EffekseerBase.h"
#include "EffekseerPosSynchro.h"
#include "PlaneEffectManeger.h"
#include "BoardPolygon.h"
#include "BoardPolygonDust.h"
#include "ScreenVibration.h"

#include "Player.h"
#include "FrameData.h"

namespace {
	constexpr int Play_Vibration = 1000;
	constexpr int Play_SE = 1001;
	constexpr int Play_CameraVibration_X = 1002;
	constexpr int Play_CameraVibration_Y = 1003;
	constexpr int Play_Effect_Dust = 1004;
	constexpr int Play_Effekseer_PC = 1005;
	constexpr int Play_Effekseer_IC = 1006;
	constexpr int Play_Effekseer_IU = 1007;
}

class ClassificationEffect
{
public:
	ClassificationEffect();
	~ClassificationEffect();
	void SetClassification(CommandParam param);
	void CreateEffeckseer(float handle, VECTOR* pos);
	static ClassificationEffect* _instance;
	static ClassificationEffect* GetInstance () { return _instance; }
protected:
	std::map<int, std::string> _commandList;
};

