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

void EnemyPool::Create(std::string createJsonFile){
	
};

void EnemyPool::Create() {
	//エネミー読み込み用ファイルができていないのでランダム

	int range = 5000;
	for (int i = 0; i < ENEMY_MAX_SIZE; i++) {
		int enemyNum = rand() % 3;
		VECTOR vPos = VGet(rand() % range - range/2, 0, rand() % range - range/2);
		switch (enemyNum) {
		case 0:
			_enemy[i] = NEW SlaBlock();
			_enemy[i]->Create(ResourceServer::MV1LoadModel("res/katatumuri/snail.mv1"), vPos, _enemyParametersMap["Slablock"]);
			break;
		case 1:
			_enemy[i] = NEW Crystarl();
			_enemy[i]->Create(ResourceServer::MV1LoadModel("res/katatumuri/snail.mv1"), vPos, _enemyParametersMap["Crystarl"]);
			break;
		case 2:
			_enemy[i] = NEW CrystarlPattern2();
			_enemy[i]->Create(ResourceServer::MV1LoadModel("res/katatumuri/snail.mv1"), vPos, _enemyParametersMap["CrystarlPattern2"]);
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