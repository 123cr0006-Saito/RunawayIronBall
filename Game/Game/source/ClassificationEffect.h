//----------------------------------------------------------------------
// @filename ClassificationEffect.h
// ＠date: 2024/02/28
// ＠author: saito ko
// @explanation
// FrameDataからのコマンドを受け取り、それに応じたエフェクトやSEを再生するクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "EffekseerBase.h"
#include "EffekseerPosSynchro.h"
#include "EffekseerRotation.h"
#include "EffectManeger.h"
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
	constexpr int Play_Effekseer_Rotation = 1008;
}

class ClassificationEffect
{
public:
	ClassificationEffect();
	~ClassificationEffect();
	void Process();
	void SetClassification(CommandParam param);
	void CreateEffeckseer(float handle, VECTOR* pos,  float height = 0.0f, VECTOR rotation = VGet(0, 0, 0));
	static ClassificationEffect* _instance;
	static ClassificationEffect* GetInstance () { return _instance; }
protected:
	Vibration* vib;
	ScreenVibration* _sVib;
	std::map<int, std::pair<std::string,int> > _commandList;
};

