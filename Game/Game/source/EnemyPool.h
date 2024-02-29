#pragma once
//ObjectPool   ����͓G�̏����f�[�^��z�u�ȂǓG�̎�ނɂ����
// �ǂݍ��񂾃f�[�^�Ŕz�u�������̂Ńe���v���[�g�ł͂Ȃ��ʂɍ�낤�Ǝv��
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
// create ��model param 
// init ��pos

class EnemyPool
{
public:
	EnemyPool(std::string paramJsonFile);
	~EnemyPool();

	void Create(myJson json);//�G�̍쐬
	void Create();//�G�̍쐬
	void Init();//�G�̔z�u�Ȃǂ̏�����
	void Init(VECTOR pos);//�G�̔z�u�@���[�v�Ȃ�
	std::vector<std::pair<std::string, VECTOR>> LoadJsonData(myJson jsonFile,std::string  loadName);//json�t�@�C������G�̏����ʒu��ǂݍ���
	void DeleteEnemy();//�G�̍폜 ���ׂď����� ����Ȃ��̂��������͖���

	EnemyBase* Recicle();//�g�p���Ă��Ȃ��I�u�W�F�N�g��Ԃ�

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

