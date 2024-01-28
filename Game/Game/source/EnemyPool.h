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
#include "Crystarl.h"

// create ��model param 
// init ��pos

class EnemyPool
{
public:
	EnemyPool(std::string paramJsonFile);
	~EnemyPool();

	void Create(std::string createJsonFile);//�G�̍쐬
	void Create();//�G�̍쐬
	void Init();//�G�̔z�u�Ȃǂ̏�����
	void Init(VECTOR pos);//�G�̔z�u�@���[�v�Ȃ�
	void DeleteEnemy();//�G�̍폜 ���ׂď����� ����Ȃ��̂��������͖���

	EnemyBase* Recicle();//�g�p���Ă��Ȃ��I�u�W�F�N�g��Ԃ�

	bool Process();
	bool Render();

private:
	static const int ENEMY_MAX_SIZE = 10;
	EnemyBase* _enemy[ENEMY_MAX_SIZE];
	std::map<std::string, EnemyParam> _enemyParametersMap;
};

