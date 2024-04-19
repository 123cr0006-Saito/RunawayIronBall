//----------------------------------------------------------------------
// @filename EnemyManeger.cpp
// ��date: 2023/12/14
// ��author: saito ko
// @explanation
// �G�l�~�[�̐����A�Ǘ����s���N���X
//----------------------------------------------------------------------
#include "EnemyManeger.h"
EnemyManeger*  EnemyManeger::_instance = nullptr;
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param paramJsonFile �p�����[�^��json�t�@�C��
//----------------------------------------------------------------------
EnemyManeger::EnemyManeger(std::string paramJsonFile){
	_instance = this;
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
		list.at("SearchRange").get_to(param._searchRange);
		list.at("DiscoverRange").get_to(param._discoverRangeSize);
		list.at("AttackRange").get_to(param._attackRangeSize);
		list.at("Suppression").get_to(param._suppression);
		_enemyParametersMap[enemyName] = param;
	}
	_enemy.clear();
	_collisionManager = CollisionManager::GetInstance();
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// ��return �Ȃ�
//----------------------------------------------------------------------
EnemyManeger::~EnemyManeger() {
	DeleteEnemy();
	_enemyParametersMap.clear();
	_enemyInitPos.clear();
	_instance = nullptr;
	_collisionManager = nullptr;
};
//----------------------------------------------------------------------
// @brief �t�@�C������G�̖��O��ǂݍ���
// ��return �ǂݍ��񂾓G�̖��O�̃��X�g
//----------------------------------------------------------------------
std::vector<std::string> EnemyManeger::LoadEnemyName(int stageNum) {
	std::vector<std::string> nameList;
	std::string filePath = "Data/LoadStageName/Enemy/Enemy0" + std::to_string(stageNum) + ".csv";
	// csv�t�@�C����ǂݍ���
	CFile file(filePath);
	// �t�@�C�����J�����ꍇ
	if (file.Success()) {
		int c = 0;
		const char* p = (const char*)file.Data();
		int size = file.Size();
		while (c < size) {
			std::string objectName;
			c += GetString(&p[c], '\r\n', &objectName, size - c); // �G�̖��O���擾
			c += SkipSpace(&p[c], &p[size]); // �󔒂�R���g���[���R�[�h���X�L�b�v����
			nameList.push_back(objectName);
		}
	}
	return nameList;
};
//----------------------------------------------------------------------
// @brief �G�̐���
// ��param json �G�̃p�����[�^��json�t�@�C��
// ��param stageNum �X�e�[�W�̔ԍ�
//----------------------------------------------------------------------
void EnemyManeger::Create(myJson json, int stageNum){
	DeleteEnemy();
	//�ǂݍ��ޓG�̖��O�̃��X�g
	int handle = 0;
	int suppression = 0;
	std::vector<std::string> enemyName = LoadEnemyName(stageNum);
	// �f�[�^�̓ǂݍ���
	for (auto&& name : enemyName) {
		std::vector<std::pair<std::string, VECTOR>> enemyData = LoadJsonData(json, name);
		suppression += _enemyParametersMap[name]._suppression * enemyData.size();
		// �f�[�^�̕��z
		for (auto& enemyDataList : enemyData) {

			EnemyBase* enemy = nullptr;

			// ������
			if (enemyDataList.first == "CryStar_Glass") {
				enemy = NEW CrystarPattern1();
				handle = ResourceServer::MV1LoadModel("CryStar_Glass","res/Enemy/Cg_Enemy_Crystar_Glass/Cg_Crystar_Glass.mv1");
			}
			else if (enemyDataList.first == "CryStar_Rock") {
				enemy = NEW CrystarPattern2();
				handle = ResourceServer::MV1LoadModel("CryStar_Rock","res/Enemy/Cg_Enemy_Crystar_Rock/Cg_Crystar_Rock.mv1");
			}
			else if (enemyDataList.first == "CryStar_Iron") {
				enemy = NEW CrystarPattern3();
				handle = ResourceServer::MV1LoadModel("CryStar_Iron","res/Enemy/Cg_Enemy_Crystar_Iron/Cg_Crystar_Iron.mv1");
			}
			else if (enemyDataList.first == "Slablock_Glass" ) {
				enemy = NEW SlaBlockPattern2();
				handle = ResourceServer::MV1LoadModel("Slablock_Glass","res/Enemy/Cg_Enemy_Slablock_Glass/Cg_Slablock_Glass.mv1");
			}
			else if (enemyDataList.first == "Slablock_Rock") {
				enemy = NEW SlaBlock();
				handle = ResourceServer::MV1LoadModel("Slablock_Rock", "res/Enemy/Cg_Enemy_Slablock_Rock/Cg_Slablock_Rock.mv1");
			}
			else if (enemyDataList.first == "Slablock_Iron") {
				enemy = NEW SlaBlock();
				handle = ResourceServer::MV1LoadModel("Slablock_Iron","res/Enemy/Cg_Enemy_Slablock_Iron/Cg_Slablock_Iron.mv1");
			}
			else if (enemyDataList.first == "ChainGuard") {
				//enemy = NEW Chainguard();
				handle = ResourceServer::MV1LoadModel("Chainguard", "res/Enemy/Cg_Enemy_ChainGuard/Cg_Enemy_ChainGuard.mv1");
			}

			// ����������Ă�����ǉ�
			if (enemy != nullptr) {
				enemy->Create(handle, enemyDataList.second, _enemyParametersMap[enemyDataList.first], enemyDataList.first);
				_enemyInitPos.emplace_back(enemyDataList.second);
				_enemy.push_back(enemy);
			}

		}
	}
	//�����l�̒ǉ�
	Suppression::GetInstance()->AddSuppression(suppression);
};
//----------------------------------------------------------------------
// @brief ������
// ��return �Ȃ�
//----------------------------------------------------------------------
void EnemyManeger::Init(){
	for (int i = 0; i < _enemy.size(); i++) {
		_enemy[i]->Init(_enemyInitPos[i]);
	}
};
//----------------------------------------------------------------------
// @brief �f�[�^�̓ǂݍ���
// ��param jsonFile �G�̍��W����������json�t�@�C��
// ��param loadName �ǂݍ��ރf�[�^�̖��O
// ��return �G�̃f�[�^
//----------------------------------------------------------------------
std::vector<std::pair<std::string, VECTOR>> EnemyManeger::LoadJsonData(myJson jsonFile, std::string  loadName) {
	nlohmann::json loadEnemy = jsonFile._json.at(loadName);
	std::vector<std::pair<std::string, VECTOR>> posList;
	for (auto& list : loadEnemy) {
		VECTOR pos;
		list.at("translate").at("x").get_to(pos.x);
		list.at("translate").at("y").get_to(pos.z);
		list.at("translate").at("z").get_to(pos.y);
		//���W�C��
		pos.x *= -1;

		posList.push_back(std::make_pair(loadName,pos));
	}
	return posList;
};
//----------------------------------------------------------------------
// @brief �G�̍폜
// ��return �Ȃ�
//----------------------------------------------------------------------
void EnemyManeger::DeleteEnemy() {
	for (auto&& enemy : _enemy) {
			delete enemy;
	}

	_enemy.clear();
	_enemyInitPos.clear();	

	ResourceServer::MV1DeleteModelAll("CryStar_Glass");
	ResourceServer::MV1DeleteModelAll("CryStar_Rock");
	ResourceServer::MV1DeleteModelAll("CryStar_Iron");
	ResourceServer::MV1DeleteModelAll("CrystarRoof_Glass");
	ResourceServer::MV1DeleteModelAll("CrystarRoof_Rock");
	ResourceServer::MV1DeleteModelAll("CrystarRoof_Iron");
	ResourceServer::MV1DeleteModelAll("Slablock");
	ResourceServer::MV1DeleteModelAll("Slablock_Iron");
	ResourceServer::MV1DeleteModelAll("Chainguard");
};

EnemyBase* EnemyManeger::Recicle() {
	for (int i = 0; i < _enemy.size() ; i++) {
		if (_enemy[i]->GetUse() == false) {
			return _enemy[i];
		}
	}
};
//----------------------------------------------------------------------
// @brief �X�V
// ��param plAttack �v���C���[�̍U���������������ǂ���
// ��return �����������ǂ���
//----------------------------------------------------------------------
bool EnemyManeger::Process(bool plAttack){
	for (auto&& enemy : _enemy) {
		if (enemy->GetUse()) {
			enemy->Process(plAttack);
			_collisionManager->UpdateCell(enemy->_cell);
		}
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// ��return �����������ǂ���
//----------------------------------------------------------------------
bool EnemyManeger::Render() {
	for (auto&& enemy : _enemy) {
		if (enemy->GetUse()) {
			enemy->Render();
		}
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �C���X�^���X�̎擾
// ��return �C���X�^���X
//----------------------------------------------------------------------
EnemyBase* EnemyManeger::GetEnemy(int i) {
	if (_enemy[i] != nullptr) {
		return _enemy[i];
	}
	return nullptr;
}