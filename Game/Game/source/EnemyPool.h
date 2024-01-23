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
#include "Crystarl.h"

// create →model param 
// init →pos

class EnemyPool
{
public:
	EnemyPool(std::string paramJsonFile);
	~EnemyPool();

	void Create(std::string createJsonFile);//敵の作成
	void Create();//敵の作成
	void Init();//敵の配置などの初期化
	void Init(VECTOR pos);//敵の配置　ループなし
	void DeleteEnemy();//敵の削除 すべて消すか いらないのを消すかは未定

	EnemyBase* Recicle();//使用していないオブジェクトを返す

	bool Process();
	bool Render();

private:
	static const int ENEMY_MAX_SIZE = 10;
	EnemyBase* _enemy[ENEMY_MAX_SIZE];
	std::map<std::string, EnemyParam> _enemyParametersMap;
};

