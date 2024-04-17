//----------------------------------------------------------------------
// @filename ModeGame.h
// ＠date: 2023/12/14
// ＠author: saito ko
// @explanation
// ボスバトル以外のステージを管理するクラス
//----------------------------------------------------------------------
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
#include "BreakableBuilding.h"
#include "Tower.h"
#include "Floor.h"
#include "UnbreakableBuilding.h"

#include "Fog.h"
#include "Light.h"
#include "Gate.h"
#include "BoardArrow.h"
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

protected:
	// その他の変数
	int _shadowHandle;// シャドウマップ用のハンドル
	bool _drawDebug = false;// デバッグ表示をするかどうか
	int _gameOverCnt;// ゲームオーバー演出のカウント
	bool transitionGameOver;// ゲームオーバー演出をし終えたか

	// 衝突判定クラス
	CollisionManager* _collisionManager;// 衝突判定クラス
	//オブジェクト関連
	int _skySphere;// スカイスフィア
	int _tile;// タイル
	int _mountain;// 山
	Player* _player;// プレイヤー
	EnemyPool* _enemyPool;// 敵
	Floor* _floor;// 床
	Gate* _gate;// ゴールゲート
	std::vector<BreakableBuilding*> _house;// 家
	std::vector<Tower*> _tower;// 塔
	std::vector<UnbreakableBuilding*> _uObj;// 破壊不可能オブジェクト
	// ステージ読み込み用変数
	std::vector<ObjectParam>_objectParam;// オブジェクトのパラメータ
	std::vector<std::string>  _objectName;// オブジェクトの名前
	//UI関連
	UIBase* ui[4];// UI　0:HP 1:EXP 2:制圧ゲージ 3:制限時間
	DrawGauge* _gaugeUI[2];// スタミナゲージUI
	int _gaugeHandle[4];// 0~2フレーム 3ゲージ
	// 演出関連
	Suppression* _suppression;// 制圧値管理クラス
	Camera* _camera;// カメラクラス
	Light* _light;// ライトクラス
	Fog* _fog;// フォグクラス
	BoardArrow* _boardArrow; //ゴールゲートを指す矢印クラス
	ClassificationEffect* _classificationEffect;// エフェクト分類クラス
	EffectManeger* _effectManeger;// エフェクト管理クラス
	TimeLimit* _timeLimit;// 制限時間クラス
	
	bool IsTutorial;// チュートリアルを表示し終えたか
};