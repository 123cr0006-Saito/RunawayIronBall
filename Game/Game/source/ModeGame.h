#pragma once
#include "appframe.h"

#include <thread>

#include "ModeClear.h"
#include "ModePause.h"
#include "ModeGameOver.h"
#include "ModeLoading.h"
#include "CollisionManager.h"

#include "Camera.h"
#include "ModeZoomCamera.h"
#include "ModeRotationCamera.h"
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

#include "Light.h"
#include "Gate.h"
#include "ClassificationEffect.h"
#include "EnemyPool.h"
#include "EffectManeger.h"

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
		int isBreak;
	};

public:

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void DeleteObject();
	std::vector<std::string> LoadObjectName(std::string fileName); // オブジェクトの名前を読み込む
	bool LoadObjectParam(std::string fileName); // オブジェクトのパラメータを読み込む
	bool LoadStage(std::string fileName);// ステージの読み込み 敵も含む
	bool StageMutation();// ステージクリア処理
	bool GateProcess();// ゴールゲートの処理


	//デバッグ用
	std::vector<OBJECTDATA> LoadJsonObject(nlohmann::json json, std::string loadName);//引数 読み込みたいオブジェクトの名前

protected:

	CollisionManager* _collisionManager;

	Camera* _camera;
	Player* _player;

	UIBase* ui[4];
	DrawGauge* _gaugeUI[2];
	int _gaugeHandle[4];// 0フレーム 3ゲージ
	float nowParcent = 100;

	TimeLimit* _timeLimit;

	ScreenVibration* _sVib;
	EnemyPool* _enemyPool;
	Suppression* _suppression;

	std::vector<House*> _house;
	std::vector<Tower*> _tower;
	std::vector<UnbreakableObject*> _uObj;
	Floor* _floor;

	int _skySphere;
	int _tile;

	int _effectSheet[30];
	Gate* _gate;
	int _stageNum;
	ClassificationEffect* _classificationEffect;
	EffectManeger* _effectManeger;
	OBB obb;

	int _shadowHandle;

	// デバッグ表示をするかどうか
	bool _drawDebug = false;



	std::vector<ObjectParam>_objectParam;

	Light* _light;

	// ステージ読み込み用変数
	bool IsLoading;
	std::thread* LoadFunctionThread;
};