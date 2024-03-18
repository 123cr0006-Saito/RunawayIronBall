#pragma once
#include "appframe.h"

#include <thread>

#include "ModePause.h"
#include "ModeGameOver.h"
#include "ModeLoading.h"
#include "CollisionManager.h"

#include "Camera.h"
#include "ModeZoomCamera.h"
#include "ModeRotationCamera.h"
#include "Player.h"
#include "Heart.h"

#include "Boss.h"

#include "UIBase.h"
#include "UIExpPoint.h"
#include "UISuppressionGauge.h"
#include "UiHeart.h"
#include "UITimeLimit.h"
#include "TimeLimit.h"

#include "ScreenVibration.h"



#include "Light.h"
#include "Gate.h"
#include "ClassificationEffect.h"
#include "EnemyPool.h"
#include "EffectManeger.h"


class ModeBossBattle : public ModeBase
{
	typedef ModeBase base;

	struct OBJECTDATA {
		std::string _name;
		VECTOR _pos;
		VECTOR _rotate;
		VECTOR _scale;
	};

	struct ObjectParam {
		std::string _name;
		VECTOR _size;
		int isBreak;
	};

public:

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();


protected:

	CollisionManager* _collisionManager;

	Camera* _camera;
	Player* _player;



	Boss* _boss;

	UIBase* ui[4];
	DrawGauge* _gaugeUI[2];
	int _gaugeHandle[4];// 0フレーム 3ゲージ
	float nowParcent = 100;

	TimeLimit* _timeLimit;

	ScreenVibration* _sVib;

	Floor* _floor;

	int _skySphere;
	int _stage;

	int _effectSheet[30];
	ClassificationEffect* _classificationEffect;
	EffectManeger* _effectManeger;
	OBB obb;

	int _shadowHandle;

	// デバッグ表示をするかどうか
	bool _drawDebug = false;
	Light* _light;

	// ステージ読み込み用変数
	bool IsLoading;
	std::thread* LoadFunctionThread;
};