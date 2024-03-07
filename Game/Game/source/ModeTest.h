#pragma once
#include "appframe.h"
#include "ModePause.h"
#include "ModeFade.h"
#include "ModeGameOver.h"

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

#include "Gate.h"
#include "ClassificationEffect.h"
#include "EnemyPool.h"
#include "EffectManeger.h"

class ModeTest : public ModeBase
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

	bool LoadObjectParam(std::string fileName); // �I�u�W�F�N�g�̃p�����[�^��ǂݍ���
	bool LoadStage(std::string fileName);// �X�e�[�W�̓ǂݍ��� �G���܂�

	bool GateProcess();// �S�[���Q�[�g�̏���

	//�f�o�b�O�p
	std::vector<OBJECTDATA> LoadJsonObject(nlohmann::json json,std::string loadName);//���� �ǂݍ��݂����I�u�W�F�N�g�̖��O

protected:

	Camera* _camera;
	Player* _player;

	Chain* _chain;
	UIBase* ui[3];
	DrawGauge* _gaugeUI[2];
	int _gaugeHandle[4];// 0�t���[�� 3�Q�[�W
	float nowParcent = 100;

	TimeLimit* _timeLimit;


	ScreenVibration* _sVib;
	EnemyPool* _enemyPool;
	Suppression* _suppression;

	std::vector<House*> _house;
	std::vector<Tower*> _tower;

	int _skySphere;
	int _tile;
	
	int _effectSheet[30];
	Gate* _gate;
	ClassificationEffect* _classificationEffect;
	EffectManeger* _effectManeger;
	OBB obb;

	int _shadowHandle;
	int _lightHandle[2];

	// �f�o�b�O�\�������邩�ǂ���
	bool _drawDebug = false;

	std::vector<std::tuple<std::string, VECTOR, int>>_objectParam;
};