#pragma once
#include "appframe.h"

#include "Camera.h"
#include "Player.h"
#include "Chain.h"
#include "UIBase.h"
#include "UIExpPoint.h"
#include "ScreenVibration.h"
#include "uiheart.h"

#include "BuildingBase.h"
#include "House.h"
#include "Tower.h"

#include "EnemyPool.h"
#include "PlaneEffectManeger.h"

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

	//�f�o�b�O�p
	std::vector<OBJECTDATA> LoadJsonObject(nlohmann::json json,std::string loadName);//���� �ǂݍ��݂����I�u�W�F�N�g�̖��O

protected:



	Camera* _camera;
	Player* _player;

	Chain* _chain;
	UIBase* ui[2];
	DrawGauge* _gaugeUI[2];
	int _gaugeHandle[4];// 0�t���[�� 3�Q�[�W
	float nowParcent = 100;

	ScreenVibration* _sVib;
	EnemyPool* _enemyPool;

	std::vector<House*> _house;
	std::vector<Tower*> _tower;

	int _skySphere;
	int _tile;
	
	int _effectSheet[30];
	PlaneEffect::PlaneEffectManeger* _planeEffectManeger;
	OBB obb;



	// �f�o�b�O�\�������邩�ǂ���
	bool _drawDebug = false;
};