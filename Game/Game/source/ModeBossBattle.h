#pragma once
#include "appframe.h"


#include "CollisionManager.h"

#include "Camera.h"
#include "Player.h"
#include "Heart.h"

#include "Boss.h"

#include "UIBase.h"
#include "UIExpPoint.h"
#include "UiHeart.h"
#include "UIBossHp.h"

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

	UIBase* ui[2];
	UIBossHp* _bossHp;
	DrawGauge* _gaugeUI[2];
	int _gaugeHandle[4];// 0フレーム 3ゲージ

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

};