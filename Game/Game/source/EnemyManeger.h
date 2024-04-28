//----------------------------------------------------------------------
// @filename EnemyManeger.cpp
// @date: 2023/12/14
// @author: saito ko
// @explanation
// �G�l�~�[�̐����A�Ǘ����s���N���X
//----------------------------------------------------------------------
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
#include "SlaBlockPattern2.h"
#include "CrystarPattern1.h"
#include "CrystarPattern2.h"
#include "CrystarPattern3.h"
#include "Suppression.h"
// create ��model param 
// init ��pos

#include "CollisionManager.h"

class EnemyManeger
{
public:
	EnemyManeger(std::string paramJsonFile);
	~EnemyManeger();

	std::vector<std::string> LoadEnemyName(int stageNum);
	void Create(myJson json,int stageNum);//�G�̍쐬
	void Init();//�G�̔z�u�Ȃǂ̏�����
	std::vector<std::pair<std::string, VECTOR>> LoadJsonData(myJson jsonFile,std::string  loadName);//json�t�@�C������G�̏����ʒu��ǂݍ���
	void DeleteEnemy();//�G�̍폜 ���ׂď����� ����Ȃ��̂��������͖���

	EnemyBase* Recicle();//�g�p���Ă��Ȃ��I�u�W�F�N�g��Ԃ�

	bool Process(bool plAttack);
	bool Render();

	static EnemyManeger* GetInstance() { return _instance; }
	static EnemyManeger* _instance;

	std::vector<EnemyBase*> GetEnemyContainer() { return _enemy; }
	EnemyBase* GetEnemy(int i);
	int GetSize() { return _enemy.size(); };

private:
	std::vector<EnemyBase*> _enemy;//�G�̃R���e�i
	std::map<std::string, EnemyParam> _enemyParametersMap;//�G�̃p�����[�^
	std::vector<VECTOR> _enemyInitPos;//�G�̏����ʒu
	CollisionManager* _collisionManager;//�����蔻����Ǘ�����N���X
};

