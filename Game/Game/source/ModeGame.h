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
#include "House.h"
#include "Tower.h"
#include "Floor.h"
#include "UnbreakableObject.h"

#include "Fog.h"
#include "Light.h"
#include "Gate.h"
#include "ClassificationEffect.h"
#include "EnemyPool.h"
#include "EffectManeger.h"

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
		int isBreak;
	};

public:

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void SetTime();
	void DeleteObject();
	std::vector<std::string> LoadObjectName(std::string fileName); // �I�u�W�F�N�g�̖��O��ǂݍ���
	bool LoadObjectParam(std::string fileName); // �I�u�W�F�N�g�̃p�����[�^��ǂݍ���
	bool LoadStage(std::string fileName);// �X�e�[�W�̓ǂݍ��� �G���܂�
	bool StageMutation();// �X�e�[�W�N���A����
	bool GateProcess();// �S�[���Q�[�g�̏���
	void NewStage();// �X�e�[�W�̏�����
	void CreateTutorial();// �`���[�g���A���̍쐬

	int GetStageNum() { return _stageNum; };


	//�f�o�b�O�p
	std::vector<OBJECTDATA> LoadJsonObject(const myJson& json, std::string loadName);//���� �ǂݍ��݂����I�u�W�F�N�g�̖��O

protected:

	CollisionManager* _collisionManager;

	Camera* _camera;
	Player* _player;

	UIBase* ui[4];
	DrawGauge* _gaugeUI[2];
	int _gaugeHandle[4];// 0�t���[�� 3�Q�[�W
	float nowParcent = 100;

	TimeLimit* _timeLimit;

	ScreenVibration* _sVib;
	EnemyPool* _enemyPool;
	Suppression* _suppression;

	std::vector<House*> _house;
	std::vector<Tower*> _tower;
	std::vector<UnbreakableObject*> _uObj;
	Floor* _floor;

	int _skySphere;
	int _tile;
	int _mountain;

	Gate* _gate;
	int _stageNum;
	ClassificationEffect* _classificationEffect;
	EffectManeger* _effectManeger;
	OBB obb;
	Fog* _fog;

	int _shadowHandle;

	// �f�o�b�O�\�������邩�ǂ���
	bool _drawDebug = false;

	std::vector<ObjectParam>_objectParam;
	std::vector<std::string>  _objectName;

	Light* _light;

	// �X�e�[�W�ǂݍ��ݗp�ϐ�
	bool IsTutorial;
};