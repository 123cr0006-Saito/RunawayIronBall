#include "EnemyPool.h"
EnemyPool::EnemyPool(std::string paramJsonFile){
	myJson json(paramJsonFile);
	EnemyParam param;
	std::string enemyName;
	for (auto&& list : json._json) {
		list.at("Id").get_to(enemyName);
		list.at("Hp").get_to(param._hp);
		list.at("Exp").get_to(param._exp);
		list.at("Speed").get_to(param._speed);
		list.at("CoolTime").get_to(param._coolTime);
		list.at("FlontAngle").get_to(param._flontAngle);
		list.at("HearingRange").get_to(param._hearingRangeSize);
		list.at("MoveRange").get_to(param._moveRange);
		list.at("SartchRange").get_to(param._sartchRange);
		list.at("DiscoverRange").get_to(param._discoverRangeSize);
		list.at("AttackRange").get_to(param._attackRangeSize);
		_enemyParametersMap[enemyName] = param;
	}
};

EnemyPool::~EnemyPool() {
	_enemyParametersMap.clear();
	delete[] _enemy; 
};

void EnemyPool::Create(myJson json){
	int i = 0;
	std::vector<std::string> enemyName = { "CryStar_Glass","CryStar_Rock", "CryStar_Iron", "Slablock_Glass","Slablock_Rock","Slablock_Iron","ChainGuard" };
	std::vector<std::pair<std::string, VECTOR>> enemyData;
	// データの読み込み
	for (auto&& name : enemyName) {
		enemyData = LoadJsonData(json, name);
	}
	// データの分配
	for (auto&& enemyDataList : enemyData) {
		int handle = 0;
		if (enemyDataList.first == "CryStar_Glass" || enemyDataList.first == "CryStar_Rock" || enemyDataList.first == "CryStar_Iron") {
			_enemy[i] = new CrystarPattern3();
			handle = ResourceServer::MV1LoadModel("res/Enemy/Crystar/cg_crystar.mv1");
		}
		else if (enemyDataList.first == "Slablock_Glass" || enemyDataList.first == "Slablock_Rock" || enemyDataList.first == "Slablock_Iron") {
			_enemy[i] = new SlaBlock();
			handle = ResourceServer::MV1LoadModel("res/Enemy/SlaBlock/SlaBlock.mv1");
		}
		else if (enemyDataList.first == "ChainGuard") {
			/*_enemy[i] = new ChainGuard();
			_enemy[i]->Create(ResourceServer::MV1LoadModel("res/Enemy/ChainGuard/ChainGuard.mv1"), vPos, _enemyParametersMap[name]);*/
		}
		_enemy[i]->Create(handle, enemyDataList.second, _enemyParametersMap[enemyDataList.first], enemyDataList.first);
		i++;
	}
	
};

void EnemyPool::Create() {
	//エネミー読み込み用ファイルができていないのでランダム

	int range = 5000;
	for (int i = 0; i < ENEMY_MAX_SIZE; i++) {
		int enemyNum = rand() % 3;
		VECTOR vPos = VGet(rand() % range - range/2, 0, rand() % range - range/2);
		switch (enemyNum) {
		case 0:
			_enemy[i] = new SlaBlock();
			_enemy[i]->Create(ResourceServer::MV1LoadModel("res/Enemy/SlaBlock/SlaBlock.mv1"), vPos, _enemyParametersMap["Slablock"],"Slablock");
			break;
		case 1:
			_enemy[i] = new CrystarPattern3();
			_enemy[i]->Create(ResourceServer::MV1LoadModel("res/Enemy/Crystar/cg_crystar.mv1"), vPos, _enemyParametersMap["Crystarl"],"Crystarl");
			break;
		case 2:
			_enemy[i] = new SlaBlockPattern2();
			_enemy[i]->Create(ResourceServer::MV1LoadModel("res/Enemy/SlaBlock/SlaBlock.mv1"), vPos, _enemyParametersMap["Slablock"],"Slablock");
			break;
		}
	}
};

void EnemyPool::Init(){
	for (int i = 0; i < ENEMY_MAX_SIZE; i++) {
		VECTOR vPos = VGet(rand() % 3000 - 1500, 0, rand() % 3000 - 1500);
		_enemy[i]->Init(vPos);
	}
};

void EnemyPool::Init(VECTOR pos) {

};

std::vector<std::pair<std::string,VECTOR>> EnemyPool::LoadJsonData(myJson jsonFile, std::string  loadName) {
	nlohmann::json loadEnemy = jsonFile._json.at(loadName);
	std::vector<std::pair<std::string, VECTOR>> posList;
	for (auto& list : loadEnemy) {
		VECTOR pos;
		list.at("translate").at("x").get_to(pos.x);
		list.at("translate").at("y").get_to(pos.z);
		list.at("translate").at("z").get_to(pos.y);
		//座標修正
		pos.x *= -1;

		posList.push_back(std::make_pair(loadName,pos));
	}
	return posList;
};

void EnemyPool::DeleteEnemy() {

};

EnemyBase* EnemyPool::Recicle() {
	for (int i = 0; i < ENEMY_MAX_SIZE; i++) {
		if (_enemy[i]->GetUse() == false) {
			return _enemy[i];
		}
	}
};

bool EnemyPool::Process(){
	for (int i = 0; i < ENEMY_MAX_SIZE; i++) {
		if (_enemy[i]->GetUse()) {
			_enemy[i]->Process();
		}
	}
	return true;
};

bool EnemyPool::Render() {
	for (int i = 0; i < ENEMY_MAX_SIZE; i++) {
		if (_enemy[i]->GetUse()) {
			_enemy[i]->Render();
		}
	}
	return true;
};