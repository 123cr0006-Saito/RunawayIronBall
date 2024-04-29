//----------------------------------------------------------------------
// @filename ModeBossBattle.h
// @date: 2024/03/20
// @author: Morozumi Hiroya
// @explanation
// �{�X�o�g�����[�h�̐�����s���N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"

#include "CollisionManager.h"

#include "Camera.h"
#include "Player.h"
#include "Heart.h"

#include "Boss.h"

#include "UIBase.h"
#include "UIExpPoint.h"
#include "UiHeart.h"
#include "UIBossHp.h"

#include "Light.h"
#include "Gate.h"
#include "ClassificationEffect.h"
#include "EnemyManeger.h"
#include "EffectManeger.h"


class ModeBossBattle : public ModeBase
{
	typedef ModeBase base;

public:

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();


protected:

	CollisionManager* _collisionManager;

	Camera* _camera;
	Player* _player;

	Boss* _boss;

	UIBase* ui[2];
	UIBossHp* _bossHp;
	DrawGauge* _gaugeUI[2];
	int _gaugeHandle[4];// 0�t���[�� 3�Q�[�W

	int _skySphere;
	int _stage;

	int _effectSheet[30];
	ClassificationEffect* _classificationEffect;
	EffectManeger* _effectManeger;

	int _shadowHandle;

	Light* _light;

};