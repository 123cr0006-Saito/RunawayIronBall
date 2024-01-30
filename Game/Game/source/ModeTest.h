#pragma once
#include "appframe.h"

#include "Camera.h"
#include "Player.h"
#include "Chain.h"
#include "UIBase.h"
#include "UIExpPoint.h"
#include "ScreenVibration.h"
#include "uiheart.h"

#include "BuildingBase.h"
#include "House.h"

#include "EnemyPool.h"
#include "PlaneEffectManeger.h"

class ModeTest : public ModeBase
{
	typedef ModeBase base;

public:

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	//デバッグ用


protected:

	Camera* _camera;
	Player* _player;

	Chain* _chain;
	UIBase* ui[2];
	DrawGauge* _gaugeUI[2];
	int _gaugeHandle[4];// 0フレーム 3ゲージ
	float nowParcent = 100;

	ScreenVibration* _sVib;
	EnemyPool* _enemyPool;

	std::vector<House*> _building;

	int _skySphere;
	int _tile;
	
	int _effectSheet[30];
	PlaneEffect::PlaneEffectManeger* _planeEffectManeger;
	OBB obb;
};