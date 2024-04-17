//----------------------------------------------------------------------
// @filename ModeGame.h
// ��date: 2023/12/14
// ��author: saito ko
// @explanation
// �{�X�o�g���ȊO�̃X�e�[�W���Ǘ�����N���X
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
	std::vector<std::string> LoadObjectName(std::string fileName); // �I�u�W�F�N�g�̖��O��ǂݍ���
	bool LoadObjectParam(std::string fileName); // �I�u�W�F�N�g�̃p�����[�^��ǂݍ���
	bool LoadStage(std::string fileName);// �X�e�[�W�̓ǂݍ��� �G���܂�
	std::vector<OBJECTDATA> LoadJsonObject(const myJson& json, std::string loadName);//���� �ǂݍ��݂����I�u�W�F�N�g�̖��O
	bool GateProcess();// �S�[���Q�[�g�̏���
	void CreateTutorial();// �`���[�g���A���̍쐬	

protected:
	// ���̑��̕ϐ�
	int _shadowHandle;// �V���h�E�}�b�v�p�̃n���h��
	bool _drawDebug = false;// �f�o�b�O�\�������邩�ǂ���
	int _gameOverCnt;// �Q�[���I�[�o�[���o�̃J�E���g
	bool transitionGameOver;// �Q�[���I�[�o�[���o�����I������

	// �Փ˔���N���X
	CollisionManager* _collisionManager;// �Փ˔���N���X
	//�I�u�W�F�N�g�֘A
	int _skySphere;// �X�J�C�X�t�B�A
	int _tile;// �^�C��
	int _mountain;// �R
	Player* _player;// �v���C���[
	EnemyPool* _enemyPool;// �G
	Floor* _floor;// ��
	Gate* _gate;// �S�[���Q�[�g
	std::vector<BreakableBuilding*> _house;// ��
	std::vector<Tower*> _tower;// ��
	std::vector<UnbreakableBuilding*> _uObj;// �j��s�\�I�u�W�F�N�g
	// �X�e�[�W�ǂݍ��ݗp�ϐ�
	std::vector<ObjectParam>_objectParam;// �I�u�W�F�N�g�̃p�����[�^
	std::vector<std::string>  _objectName;// �I�u�W�F�N�g�̖��O
	//UI�֘A
	UIBase* ui[4];// UI�@0:HP 1:EXP 2:�����Q�[�W 3:��������
	DrawGauge* _gaugeUI[2];// �X�^�~�i�Q�[�WUI
	int _gaugeHandle[4];// 0~2�t���[�� 3�Q�[�W
	// ���o�֘A
	Suppression* _suppression;// �����l�Ǘ��N���X
	Camera* _camera;// �J�����N���X
	Light* _light;// ���C�g�N���X
	Fog* _fog;// �t�H�O�N���X
	BoardArrow* _boardArrow; //�S�[���Q�[�g���w�����N���X
	ClassificationEffect* _classificationEffect;// �G�t�F�N�g���ރN���X
	EffectManeger* _effectManeger;// �G�t�F�N�g�Ǘ��N���X
	TimeLimit* _timeLimit;// �������ԃN���X
	
	bool IsTutorial;// �`���[�g���A����\�����I������
};