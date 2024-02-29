#pragma once
#include "appframe.h"
#include "ModePause.h"
#include "ModeGameOver.h"

#include "Camera.h"
#include "Player.h"
#include "Chain.h"

#include "UIBase.h"
#include "UIExpPoint.h"
#include "UISuppressionGauge.h"
#include "UiHeart.h"
#include "UITimeLimit.h"
#include "TimeLimit.h"

#include "ScreenVibration.h"

#include "BuildingBase.h"
#include "House.h"
#include "Tower.h"

#include "EnemyPool.h"
#include "PlaneEffectManeger.h"

#include "Boss.h"

class ModeTest : public ModeBase
{
	typedef ModeBase base;

	struct OBJECTDATA {
		std::string _name;
		VECTOR _pos;
		VECTOR _rotate;
		VECTOR _scale;
	};

public:

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	//デバッグ用
	std::vector<OBJECTDATA> LoadJsonObject(nlohmann::json json,std::string loadName);//引数 読み込みたいオブジェクトの名前

protected:

	Camera* _camera;
	Player* _player;

	Chain* _chain;
	UIBase* ui[4];
	DrawGauge* _gaugeUI[2];
	int _gaugeHandle[4];// 0フレーム 3ゲージ
	float nowParcent = 100;

	TimeLimit* _timeLimit;


	ScreenVibration* _sVib;
	EnemyPool* _enemyPool;

	Boss* _boss;

	std::vector<House*> _house;
	std::vector<Tower*> _tower;

	int _skySphere;
	int _tile;
	
	int _effectSheet[30];
	PlaneEffect::PlaneEffectManeger* _planeEffectManeger;
	OBB obb;

	int _shadowHandle;
	int _lightHandle[2];

	// デバッグ表示をするかどうか
	bool _drawDebug = false;
};