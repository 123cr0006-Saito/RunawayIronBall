#pragma once
#include "appframe.h"

#include <thread>
#include "CollisionManager.h"

#include "Camera.h"
#include "Player.h"
#include "Heart.h"

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
#include "Floor.h"
#include "UnbreakableObject.h"

#include "Fog.h"
#include "Light.h"
#include "Gate.h"
#include "ClassificationEffect.h"
#include "EnemyPool.h"
#include "EffectManeger.h"

const float STAGE_ONE_WIDTH  = 17000.0f/2.0f;
const float STAGE_TWO_WIDTH  = 20000.0f/2.0f;
const float STAGE_THREE_WIDTH = 27000.0f/2.0f;

class ModeGame : public ModeBase
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
		int _hp;
		int _exp;
		int _suppression;
		int isBreak;
	};

public:

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void SetTime();
	std::vector<std::string> LoadObjectName(std::string fileName); // オブジェクトの名前を読み込む
	bool LoadObjectParam(std::string fileName); // オブジェクトのパラメータを読み込む
	bool LoadStage(std::string fileName);// ステージの読み込み 敵も含む
	std::vector<OBJECTDATA> LoadJsonObject(const myJson& json, std::string loadName);//引数 読み込みたいオブジェクトの名前
	bool GateProcess();// ゴールゲートの処理
	void CreateTutorial();// チュートリアルの作成

	int GetStageNum() { return global._stageNum; };

	

protected:

	CollisionManager* _collisionManager;

	Camera* _camera;
	Player* _player;
	int _gameOverCnt;
	bool transitionGameOver;

	UIBase* ui[4];
	DrawGauge* _gaugeUI[2];
	int _gaugeHandle[4];// 0フレーム 3ゲージ

	TimeLimit* _timeLimit;

	EnemyPool* _enemyPool;
	Suppression* _suppression;

	std::vector<House*> _house;
	std::vector<Tower*> _tower;
	std::vector<UnbreakableObject*> _uObj;
	Floor* _floor;

	int _skySphere;
	int _tile;
	int _mountain;

	Gate* _gate;
	ClassificationEffect* _classificationEffect;
	EffectManeger* _effectManeger;
	OBB obb;
	Fog* _fog;

	int _shadowHandle;

	// デバッグ表示をするかどうか
	bool _drawDebug = false;

	std::vector<ObjectParam>_objectParam;
	std::vector<std::string>  _objectName;

	Light* _light;

	// ステージ読み込み用変数
	bool IsTutorial;
};