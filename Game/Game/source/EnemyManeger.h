//----------------------------------------------------------------------
// @filename EnemyManeger.cpp
// @date: 2023/12/14
// @author: saito ko
// @explanation
// エネミーの生成、管理を行うクラス
//----------------------------------------------------------------------
#pragma once
//ObjectPool   今回は敵の初期データや配置など敵の種類によって
// 読み込んだデータで配置したいのでテンプレートではなく個別に作ろうと思う
#include <vector>
#include <map>
#include <string>
#include "myJson.h"

#include "EnemyStract.h"
#include "EnemyBase.h"
#include "SlaBlock.h"
#include "SlaBlockPattern2.h"
#include "CrystarPattern1.h"
#include "CrystarPattern2.h"
#include "CrystarPattern3.h"
#include "Suppression.h"
// create →model param 
// init →pos

#include "CollisionManager.h"

class EnemyManeger
{
public:
	EnemyManeger(std::string paramJsonFile);
	~EnemyManeger();

	std::vector<std::string> LoadEnemyName(int stageNum);
	void Create(myJson json,int stageNum);//敵の作成
	void Init();//敵の配置などの初期化
	std::vector<std::pair<std::string, VECTOR>> LoadJsonData(myJson jsonFile,std::string  loadName);//jsonファイルから敵の初期位置を読み込む
	void DeleteEnemy();//敵の削除 すべて消すか いらないのを消すかは未定

	EnemyBase* Recicle();//使用していないオブジェクトを返す

	bool Process(bool plAttack);
	bool Render();

	static EnemyManeger* GetInstance() { return _instance; }
	static EnemyManeger* _instance;

	std::vector<EnemyBase*> GetEnemyContainer() { return _enemy; }
	EnemyBase* GetEnemy(int i);
	int GetSize() { return _enemy.size(); };

private:
	std::vector<EnemyBase*> _enemy;//敵のコンテナ
	std::map<std::string, EnemyParam> _enemyParametersMap;//敵のパラメータ
	std::vector<VECTOR> _enemyInitPos;//敵の初期位置
	CollisionManager* _collisionManager;//当たり判定を管理するクラス
};

