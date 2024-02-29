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
#include "CrystarPattern1.h"
#include "CrystarPattern2.h"
#include "CrystarPattern3.h"
#include "SlaBlockPattern2.h"
#include "Suppression.h"
// create →model param 
// init →pos

class EnemyPool
{
public:
	EnemyPool(std::string paramJsonFile);
	~EnemyPool();

	void Create(myJson json);//敵の作成
	void Create();//敵の作成
	void Init();//敵の配置などの初期化
	void Init(VECTOR pos);//敵の配置　ループなし
	std::vector<std::pair<std::string, VECTOR>> LoadJsonData(myJson jsonFile,std::string  loadName);//jsonファイルから敵の初期位置を読み込む
	void DeleteEnemy();//敵の削除 すべて消すか いらないのを消すかは未定

	EnemyBase* Recicle();//使用していないオブジェクトを返す

	bool Process();
	bool Render();

	static EnemyPool* GetInstance() { return _instance; }
	static EnemyPool* _instance;

	EnemyBase* GetEnemy(int i);

	static const int ENEMY_MAX_SIZE = 300;
private:
	EnemyBase* _enemy[ENEMY_MAX_SIZE];
	std::map<std::string, EnemyParam> _enemyParametersMap;
	std::vector<VECTOR> _enemyInitPos;

	
};

