#pragma once
#include "appframe.h"

#include <thread>

#include "ModePause.h"
#include "ModeGameOver.h"
#include "ModeLoading.h"
#include "CollisionManager.h"

#include "Camera.h"
#include "Player.h"
#include "Chain.h"

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

public:

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void DeleteObject();
	std::vector<std::string> LoadObjectName(std::string fileName); // �I�u�W�F�N�g�̖��O��ǂݍ���
	bool LoadObjectParam(std::string fileName); // �I�u�W�F�N�g�̃p�����[�^��ǂݍ���
	bool LoadStage(std::string fileName);// �X�e�[�W�̓ǂݍ��� �G���܂�
	bool StageMutation();// �X�e�[�W�N���A����
	bool GateProcess();// �S�[���Q�[�g�̏���


	//�f�o�b�O�p
	std::vector<OBJECTDATA> LoadJsonObject(nlohmann::json json, std::string loadName);//���� �ǂݍ��݂����I�u�W�F�N�g�̖��O

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

	int _effectSheet[30];
	Gate* _gate;
	int _stageNum;
	ClassificationEffect* _classificationEffect;
	EffectManeger* _effectManeger;
	OBB obb;

	int _shadowHandle;

	// �f�o�b�O�\�������邩�ǂ���
	bool _drawDebug = false;

	std::vector<std::tuple<std::string, VECTOR, int>>_objectParam;

	Light* _light;

	// �X�e�[�W�ǂݍ��ݗp�ϐ�
	bool IsLoading;
	std::thread* LoadFunctionThread;
};