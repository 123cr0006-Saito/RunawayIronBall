#pragma once
#include "appframe.h"

#include "Camera.h"
#include "Player.h"
#include "Chain.h"
#include "UIBase.h"
#include "uiheart.h"

#include "BuildingBase.h"
#include "House.h"

class ModeTest : public ModeBase
{
	typedef ModeBase base;

public:

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();


protected:
	Camera* _camera;
	Vibration* _vibration;

	Player* _player;

	Chain* _chain;
	UIBase* ui;

	std::vector<House*> _building;

	int _skySphere;


	OBB obb;
};