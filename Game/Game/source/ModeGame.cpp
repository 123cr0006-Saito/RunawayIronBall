#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeTest.h"

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }

	_collisionManager = NEW CollisionManager();
	_collisionManager->Init();

	_gate = nullptr;
	_stageNum = 1;
	IsLoading = true;
	LoadFunctionThread = nullptr;

	_light = NEW Light("LightData");
	
	int resolution = 8192;
	_shadowHandle = MakeShadowMap(resolution, resolution);

	_skySphere = MV1LoadModel(_T("res/SkySphere/skysphere.mv1"));
	_tile = MV1LoadModel(_T("res/TemporaryMaterials/stage_normal_01.mv1"));
	MV1SetPosition(_skySphere, VGet(0, 0, 0));
	MV1SetScale(_skySphere, VGet(3, 3, 3));
	MV1SetPosition(_tile, VGet(0, 0, 0));

	int playerModelHandle = ResourceServer::MV1LoadModel("Player", "res/Character/cg_player_girl/cg_player_girl_TEST_Ver.2.mv1");
	_player = NEW Player(playerModelHandle, VGet(0, 0, 0));
	_camera = NEW Camera(_player->GetPosition());


	_classificationEffect = NEW ClassificationEffect();
	_effectManeger = NEW EffectManeger();

	{
		ResourceServer::Load("FX_3D_Level_Up", "res/Effekseer/FX_3D_Level_Up/FX_3D_Level_Up.efkefc");
		ResourceServer::Load("Stanp", "res/Effekseer/Attack/HorizontalThird.efkefc");
		ResourceServer::Load("Rotation", "res/Effekseer/FX_3D_Rotate_2/FX_3D_Rotate.efkefc");
		ResourceServer::Load("SlashR", "res/Effekseer/Slash/SlashRight.efkefc");
		ResourceServer::Load("SlashL", "res/Effekseer/Slash/SlashLeft.efkefc");
		ResourceServer::LoadMultGraph("split", "res/TemporaryMaterials/split/test", ".png", 30, _effectSheet);
		ResourceServer::LoadDivGraph("Dust", "res/TemporaryMaterials/FX_Dust_2D.png", 44, 20, 3, 1000, 1000);
		ResourceServer::LoadEffekseerEffect("Stanp", "res/Effekseer/Attack/HorizontalThird.efkefc");
	}

	_suppression = NEW Suppression();

	_enemyPool = NEW EnemyPool("res/JsonFile/EnemyData.json");

	// �I�u�W�F�N�g�̃f�[�^�̓ǂݍ���
	LoadObjectParam("BuildingtList.csv");
	// �X�e�[�W�̃f�[�^�̓ǂݍ���
	LoadStage("Data/ObjectList/Stage_03.json");

	int size = 100;
	int heartHandle[3];
	ResourceServer::LoadMultGraph("Heart", "res/UI/UI_Heart", ".png", 3, heartHandle);
	ui[0] = NEW UIHeart(VGet(20, 20, 0), 3, heartHandle, 2);
	ui[1] = NEW UIExpPoint(VGet(0, 150, 0));
	ResourceServer::LoadMultGraph("Suppressiongauge", "res/TemporaryMaterials/SuppressionGauge/suppressiongauge", ".png", 3, heartHandle);
	ui[2] = NEW UISuppressionGauge(VGet(700, 100, 0), 3, heartHandle);
	_gaugeUI[0] = NEW DrawGauge(0, 3, size, true);
	_gaugeUI[1] = NEW DrawGauge(0, 3, size, true);
	_gaugeHandle[0] = ResourceServer::LoadGraph("Stamina03", _T("res/UI/UI_Stamina_03.png"));
	_gaugeHandle[1] = ResourceServer::LoadGraph("Stamina02", _T("res/UI/UI_Stamina_02.png"));
	_gaugeHandle[2] = ResourceServer::LoadGraph("Stamina01", _T("res/UI/UI_Stamina_01.png"));
	_gaugeHandle[3] = ResourceServer::LoadGraph("Stamina04", _T("res/UI/UI_Stamina_04.png"));
	_sVib = NEW ScreenVibration();

	

	//global._soundServer->DirectPlay("Stage03");
	global._soundServer->BgmFadeIn("Stage03", 2000);


	TowerParts::InitBlastTowerParts();

	return true;
}

bool ModeGame::Terminate() {
	base::Terminate();
	delete _camera;
	delete _player;
	delete _sVib;
	delete _enemyPool;
	delete _suppression;
	delete _effectManeger;
	delete _classificationEffect;
	delete _light;

	if (LoadFunctionThread != nullptr) {
		delete LoadFunctionThread;
	}

	if (_gate != nullptr) {
		delete _gate;
	}

	for (int i = 0; i < 3; i++) {
		delete ui[i];
	}

	for (int i = 0; i < 2; i++) {
		delete _gaugeUI[i];
	}

	for (auto&& house : _house) {
		delete house;
	}

	for (auto&& tower : _tower) {
		delete tower;
	}

	for (auto&& uObj : _uObj) {
		delete uObj;
	}

	return true;
}

std::vector<ModeGame::OBJECTDATA> ModeGame::LoadJsonObject(nlohmann::json json, std::string loadName) {
	nlohmann::json loadObject = json.at(loadName);
	std::vector<ModeGame::OBJECTDATA> _objectList;
	for (auto& list : loadObject) {
		OBJECTDATA object;
		list.at("objectName").get_to(object._name);
		list.at("translate").at("x").get_to(object._pos.x);
		list.at("translate").at("y").get_to(object._pos.z);
		list.at("translate").at("z").get_to(object._pos.y);
		list.at("rotate").at("x").get_to(object._rotate.x);
		list.at("rotate").at("y").get_to(object._rotate.z);
		list.at("rotate").at("z").get_to(object._rotate.y);
		list.at("scale").at("x").get_to(object._scale.x);
		list.at("scale").at("y").get_to(object._scale.z);
		list.at("scale").at("z").get_to(object._scale.y);
		//���W�C��
		object._pos.x *= -1;
		//degree->radian
		object._rotate.x = object._rotate.x * DX_PI_F / 180;
		object._rotate.y = (object._rotate.y + 180) * DX_PI_F / 180;
		object._rotate.z = object._rotate.z * DX_PI_F / 180;
		_objectList.push_back(object);
	}
	return _objectList;
};

bool ModeGame::LoadObjectParam(std::string fileName) {
	std::vector<std::tuple<std::string, VECTOR, bool> > paramList;
	std::string filePath = "Data/BuildingData/" + fileName;
	// csv�t�@�C����ǂݍ���
	CFile file(filePath);
	// �t�@�C�����J�����ꍇ
	if (file.Success()) {
		int c = 0;
		const char* p = (const char*)file.Data();
		int size = file.Size();
		while (c < size) {
			std::string modelName;
			int x, y, z;
			int IsBreaking;
			c += GetString(&p[c], &modelName); // ���f���̖��O���擾
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &x); // obb��X�T�C�Y���擾
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &y); // obb��Y�T�C�Y���擾
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &z); // obb��Z�T�C�Y���擾
			c += FindString(&p[c], ',', &p[size]); c++; c += GetDecNum(&p[c], &IsBreaking); // �j��\��
			c += SkipSpace(&p[c], &p[size]); // �󔒂�R���g���[���R�[�h���X�L�b�v����
			_objectParam.push_back(std::make_tuple(modelName, VGet(x, y, z), IsBreaking));
		}
	}
	else {
		return false;
	}
	return true;
};

std::vector<std::string> ModeGame::LoadObjectName(std::string fileName) {
	std::vector<std::string> nameList;
	std::string filePath = "Data/LoadStageName/" + fileName + "/" + fileName + std::to_string(_stageNum) + ".csv";
	// csv�t�@�C����ǂݍ���
	CFile file(filePath);
	// �t�@�C�����J�����ꍇ
	if (file.Success()) {
		int c = 0;
		const char* p = (const char*)file.Data();
		int size = file.Size();
		while (c < size) {
			std::string objectName;
			c += GetString(&p[c], '\r\n', &objectName); // ���f���̖��O���擾
			c += SkipSpace(&p[c], &p[size]); // �󔒂�R���g���[���R�[�h���X�L�b�v����
			nameList.push_back(objectName);
		}
	}
	return nameList;
}

bool ModeGame::LoadStage(std::string fileName) {
	myJson json(fileName);

	_enemyPool->Create(json);

	std::string buildingName = "Building";
	std::vector<std::string> objectName = LoadObjectName(buildingName);
	for (auto&& nameList : objectName) {

		auto itr = std::find_if(_objectParam.begin(), _objectParam.end(), [=](std::tuple<std::string, VECTOR, int> temp)
		{
				return std::get<0>(temp) == nameList;
		});

		std::vector<ModeGame::OBJECTDATA> objectData = LoadJsonObject(json._json, nameList);
		std::string modelName = "res/Building/" + std::get<0>((*itr)) + "/" + std::get<0>((*itr)) + ".mv1";
		int objHandle = MV1LoadModel(modelName.c_str());
		for (auto&& object : objectData) {
			if (std::get<2>((*itr)) == 1) {
				// ����I�u�W�F�N�g
				House* building = NEW House();
				building->Init(MV1DuplicateModel(objHandle), object._pos, object._rotate, object._scale, std::get<1>((*itr)));
				_house.push_back(building);
			}
			else {
				// ���Ȃ��I�u�W�F�N�g
				UnbreakableObject* uObj = NEW UnbreakableObject();
				uObj->Init(MV1DuplicateModel(objHandle), object._pos, object._rotate, object._scale, std::get<1>((*itr)));
				_uObj.push_back(uObj);
			}
		}
	}

	// �^���[
	for (int i = 0; i < 5; i++) {
		VECTOR v = VGet(rand() % 8000, 0.0f, rand() % 8000);
		v.x -= 4000.0f;
		v.z -= 4000.0f;

		std::array<int, 3> towerModelHandle;
		towerModelHandle[0] = ResourceServer::MV1LoadModel("Tower01", "res/Building/CG_OBJ_Tower/CG_OBJ_Tower_Under.mv1");
		towerModelHandle[1] = ResourceServer::MV1LoadModel("Tower02", "res/Building/CG_OBJ_Tower/CG_OBJ_Tower_Under.mv1");
		towerModelHandle[2] = ResourceServer::MV1LoadModel("Tower03", "res/Building/CG_OBJ_Tower/CG_OBJ_Tower_Top.mv1");

		Tower* tower = NEW Tower();
		tower->Init(towerModelHandle, v, VGet(0, 0, 0), VGet(1, 1, 1));

		_tower.push_back(tower);
	}

	return true;
};

bool ModeGame::StageMutation() {
	// ���[�h�J�n
	IsLoading = false;

	// ���g������Ȃ����̂�delete����
	delete _gate; _gate = nullptr;
	_enemyPool->DeleteEnemy();


    // �I�u�W�F�N�g�̃f�[�^�̓ǂݍ��� �t�@�C������ 1 ����n�܂�̂� +1 ����
	std::string fileName = "Data/ObjectList/Stage_0" + std::to_string(_stageNum + 1) + ".json";
	LoadStage(fileName);

	IsLoading = true;
	LoadFunctionThread->detach();
	delete LoadFunctionThread; LoadFunctionThread = nullptr;
	// ���[�h�I��
	return true;
}

bool ModeGame::Process() {
	base::Process();

	bool isAttackState = _player->GetEnabledIBAttackCollision();
	bool isInvincible = _player->GetIsInvincible();
	VECTOR pPos = _player->GetPosition();


	Sphere ibSphere = _player->GetIBCollision();

	int ibPower = _player->GetPower();


	Capsule plCol = _player->GetCollision();
	Sphere ibCol = _player->GetIBCollision();

	global._timer->TimeElapsed();
	_sVib->UpdateScreenVibration();

	_player->Process(_camera->GetCamY());
	_enemyPool->Process(isAttackState);

	for (int i = 0; i < sizeof(ui) / sizeof(ui[0]); i++) {
		ui[i]->Process();
	}



	for (auto itr = _house.begin(); itr != _house.end(); ++itr) {
		(*itr)->Process();

		if ((*itr)->GetUseCollision()) {
			OBB houseObb = (*itr)->GetOBBCollision();

			if (Collision3D::OBBSphereCol(houseObb, ibSphere)) {
				if (isAttackState) {
					VECTOR vDir = VSub(houseObb.pos, pPos);
					(*itr)->ActivateBreakObject(true, vDir);
					_player->SetExp(50);
					global._soundServer->DirectPlay("OBJ_RockBreak");
					continue;
				}
			}

			//�G�l�~�[���m�b�N�o�b�N��Ԃ̎��A�����ɂԂ�������j�󂷂�
			houseObb.pos.y = 0; houseObb.length[1] = 0; //���ʂł̓����蔻��̂��ߌ�����y���̒�����0�ɂ���]
			int enemySize = _enemyPool->GetSize();
			for (int i = 0; i < enemySize; i++) {
				EnemyBase* en = _enemyPool->GetEnemy(i);
				if (!en) { continue; }
				if (!en->GetUse()) { continue; }

				ENEMYTYPE enState = en->GetEnemyState();

				VECTOR enPos = en->GetCollisionPos(); enPos.y = 0;
				VECTOR hitPos = VGet(0, 0, 0);
				float enR = en->GetR();

				if (Collision3D::OBBSphereCol(houseObb, enPos, enR, &hitPos)) {
					if (enState == ENEMYTYPE::DEAD) {
						VECTOR vDir = VSub(houseObb.pos, pPos);
						(*itr)->ActivateBreakObject(true, vDir);
						global._soundServer->DirectPlay("OBJ_RockBreak");
						continue;
					}
					else {
						VECTOR dirVec = VSub(enPos, hitPos);
						dirVec = VNorm(dirVec);
						VECTOR movePos = VAdd(hitPos, VScale(dirVec, enR));
						en->SetPos(movePos);
					}
				}
			}

			for (auto tpItr = TowerParts::_blastTowerParts.begin(); tpItr != TowerParts::_blastTowerParts.end(); ++tpItr) {
				Sphere tpSphere = (*tpItr)->GetSphereCollision();
				if (Collision3D::OBBSphereCol(houseObb, tpSphere)) {
					VECTOR vDir = VSub(houseObb.pos, tpSphere.centerPos);
					(*itr)->ActivateBreakObject(true, vDir);
					continue;
				}
			}

			// �v���C���[�̉��o
			VECTOR hitPos = VGet(0, 0, 0);
			VECTOR pPos = _player->GetPosition();
			float pPosY = pPos.y;
			pPos.y = 0;
			float pR = _player->GetCollision().r;
			if (Collision3D::OBBSphereCol(houseObb, pPos, pR, &hitPos)) {
				VECTOR dirVec = VSub(pPos, hitPos);
				dirVec = VNorm(dirVec);
				VECTOR movePos = VAdd(hitPos, VScale(dirVec, pR));
				_player->SetPos(movePos);

			}
		}
	}

	for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
		(*itr)->Process();

		if ((*itr)->GetUse()) {
			VECTOR tPos = (*itr)->GetPos();
			Sphere tSphere = (*itr)->GetBottomSphereCollision();
			if ((*itr)->GetCanBlast()) {
				if (isAttackState) {
					if (Collision3D::SphereCol(ibSphere, tSphere)) {

						VECTOR vDir = VSub(tPos, pPos);
						(*itr)->SetBlast(vDir);
					}
				}
			}

			// �G�l�~�[�̉��o����
			int enemySize = _enemyPool->GetSize();
			for (int i = 0; i < enemySize; i++) {
				EnemyBase* en = _enemyPool->GetEnemy(i);
				if (!en) { continue; }
				if (!en->GetUse()) { continue; }

				float tR = tSphere.r;
				VECTOR enPos = en->GetCollisionPos();
				float enR = en->GetR();

				enPos.y = 0;
				tSphere.centerPos.y = 0;

				VECTOR vDir = VSub(enPos, tSphere.centerPos);
				if (VSize(vDir) <= enR + tR) {
					float len = (enR + tR) - VSize(vDir);
					vDir = VNorm(vDir);
					en->SetPos(VAdd(enPos, VScale(vDir, len)));
				}
				en = nullptr;
			}

			// �v���C���[�̉��o
			VECTOR pColPos = _player->GetPosition(); pColPos.y = 0;
			float pR = _player->GetCollision().r;
			float tR = tSphere.r;


			VECTOR vDir = VSub(pColPos, tSphere.centerPos);
			if (VSize(vDir) <= pR + tR) {
				float len = (pR + tR) - VSize(vDir);
				vDir = VNorm(vDir);
				_player->SetPos(VAdd(pColPos, VScale(vDir, len)));
			}
		}
	}

	int enemySize = _enemyPool->GetSize();
	for (int i = 0; i < enemySize; i++) {
		EnemyBase* enemy = _enemyPool->GetEnemy(i);
		if (!enemy) { continue; }
		if (!enemy->GetUse()) { continue; }
		if (isAttackState) {

			VECTOR enPos = enemy->GetCollisionPos();
			float enR = enemy->GetR();

			if (Collision3D::SphereCol(ibSphere.centerPos, ibSphere.r, enPos, enR)) {
				VECTOR vDir = VSub(enPos, pPos);
				vDir = VNorm(vDir);
				enemy->SetKnockBackAndDamage(vDir, ibPower);
			}
		}
		// �G�ƃv���C���[�̓����蔻��

		Sphere eCol = { enemy->GetCollisionPos(), enemy->GetR() };
		Capsule pCol = _player->GetCollision();
		if (Collision3D::SphereCapsuleCol(eCol, pCol)) {
			if (!isInvincible) {
				_player->SetDamage();
			}
			VECTOR tmpPos = enemy->GetCollisionPos();
			tmpPos.y = 0.0f;

			VECTOR vDir = VSub(pCol.down_pos, tmpPos);
			vDir.y = 0.0f;
			float squareLength = VSquareSize(vDir);
			if (squareLength >= 0.0001f) {
				vDir = VNorm(vDir);
				tmpPos = VAdd(tmpPos, VScale(vDir, eCol.r + pCol.r));
				_player->SetPos(tmpPos);
			}
			enemy = nullptr;


		}
	}

	//��ԕ������l���Ă��Ȃ��̂Ŗ��ʂ������ł��B
	for (int i = 0; i < enemySize; i++) {
		EnemyBase* en = _enemyPool->GetEnemy(i);
		if (!en) { continue; }
		if (!en->GetUse()) { continue; }

		VECTOR en1Pos = en->GetCollisionPos();
		float en1R = en->GetR();
		for (int j = 0; j < enemySize; j++) {
			if (i == j) { continue; }

			EnemyBase* en = _enemyPool->GetEnemy(j);
			if (!en) { continue; }
			if (!en->GetUse()) { continue; }

			VECTOR en2Pos = en->GetCollisionPos();
			float en2R = en->GetR();
			VECTOR dirVec = VSub(en2Pos, en1Pos);
			float length = VSize(dirVec);
			if (length <= en1R + en2R) {
				float pushLength = (en1R + en2R) - length;
				dirVec = VNorm(dirVec);
				en->SetExtrusionPos(VScale(dirVec, pushLength));
			}
		}
	}



	if (XInput::GetInstance()->GetTrg(XINPUT_BUTTON_START)) {
		//_enemyPool->Init();
		//_player->SetDamage();
		ModeServer::GetInstance()->Add(NEW ModePause(), 10, "Pause");
	}

	if (XInput::GetInstance()->GetKey(XINPUT_BUTTON_Y)) {
		if (nowParcent > 0) {
			nowParcent -= 1.0f / 120 * 100;
		}
	}
	else {
		if (nowParcent < 100) {
			nowParcent += 1.0f / 120 * 100;
		}
	}

	if (XInput::GetInstance()->GetTrg(XINPUT_BUTTON_BACK)) {
		_drawDebug = !_drawDebug;
		//VECTOR pPos = _player->GetPosition();
		//for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
		//	
		//	VECTOR hPos = (*itr)->GetPos();
		//	VECTOR tmpDir = VSub(hPos, pPos);
		//	tmpDir.y = 0.0f;
		//	(*itr)->SetBlast(tmpDir);
		//}
	}

	if (_player->GetHP() <= 0) {
		global._soundServer->BgmFadeOut(2000);
		ModeServer::GetInstance()->Del(this);
		ModeServer::GetInstance()->Add(NEW ModeGameOver(), 1, "gameover");
	}

	VECTOR box_vec = ConvWorldPosToScreenPos(VAdd(_player->GetPosition(), VGet(0, 170, 0)));

	float ratio = 1.0f - _camera->GetTargetDistance() / _camera->GetMaxLength();
	_gaugeUI[0]->Process(box_vec, _player->GetStamina(), _player->GetStaminaMax(),ratio);
	_gaugeUI[1]->Process(box_vec, 100, 100,ratio);

	_player->AnimationProcess();

	TowerParts::CheckFinishedBlastTowerParts();

	_effectManeger->Update();
	_camera->Process(_player->GetPosition(), _tile);

	GateProcess();

	return true;
}


bool ModeGame::GateProcess() {
	_suppression->SubSuppression(1);
	if (_suppression->GetIsRatio()) {
		if (_gate == nullptr) {
			int handle[43];
			ResourceServer::LoadDivGraph("Gate", "res/TemporaryMaterials/FX_Hole_2D00_sheet.png", 43, 16, 3, 1200, 1200, handle);
			float time = 1.0f / 60.0f * 1000.0f;
			_gate = NEW Gate(VGet(0, 300, 0), 300, handle, 43, time, 1000);
		}
		_gate->Process();

		VECTOR pPos = _player->GetPosition();
		float pR = _player->GetCollision().r;
		VECTOR gPos = _gate->GetPos();
		float gR = _gate->GetR();

		// �S�[���Q�[�g�̓����蔻��
		if (Collision3D::SphereCol(pPos, pR, gPos, gR)) {
			// �����Ń��A���^�C�������_�����O�̃A�j���[�V����������
			// ���ꂪ�I�������t�F�[�h�A�E�g�@���@���[�f�B���O�E�]���i�����@���ԁj�@���@���[�f�B���O���I������玟�̃X�e�[�W
			// ���͂����ɃX�e�[�W�@�ۊ֐���ǉ�
			int time = 4 * 1000; // 4�b
			_stageNum++;
			LoadFunctionThread = NEW std::thread(&ModeGame::StageMutation, this);
			ModeServer::GetInstance()->Add(NEW ModeLoading(&IsLoading, LoadFunctionThread), 100, "Loading");
		}
	}
	return true;
};// �S�[���Q�[�g�̏���



bool ModeGame::Render() {
	if (LoadFunctionThread)return true;

	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	MV1DrawModel(_skySphere);

	// �`��Ɏg�p����V���h�E�}�b�v��ݒ�
	SetUseShadowMap(0, _shadowHandle);
	MV1DrawModel(_tile);
	// �`��Ɏg�p����V���h�E�}�b�v�̐ݒ������
	SetUseShadowMap(0, -1);


	// ���C�g�ݒ�
	SetUseLighting(TRUE);

	// 0,0,0�𒆐S�ɐ�������
	{
		float linelength = 1000.f;
		VECTOR v = { 0, 0, 0 };
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}

	//------------------------------------
	// �V���h�E�}�b�v�̐ݒ�@
	// shadowCount 0 �V���h�E�}�b�v�ɕ`�� 1 ���f���̕`��
	VECTOR lightDir = VGet(-0.5f, -1.0f, -0.5f);
	SetShadowMapLightDirection(_shadowHandle, lightDir);

	// �V���h�E�}�b�v�ɕ`�悷��͈͂�ݒ�
	// �J�����̒����_�𒆐S�ɂ���
	float length = 10000.f;
	VECTOR plPos = _player->GetPosition();
	SetShadowMapDrawArea(_shadowHandle, VAdd(plPos, VGet(-length, -1.0f, -length)), VAdd(plPos, VGet(length, length, length)));

	for (int shadowCount = 0; shadowCount < 2; shadowCount++) {
		// �V���h�E�}�b�v�̐ݒ�
		if (shadowCount == 0) {
			// �V���h�E�}�b�v�ւ̕`��̏���
			ShadowMap_DrawSetup(_shadowHandle);
		}
		else if (shadowCount == 1) {
			// �V���h�E�}�b�v�ւ̕`����I��
			ShadowMap_DrawEnd();

		}

		//-------------------------------------------------------------------------------------

		_player->Render();
		_enemyPool->Render();
		//_chain->DrawDebugInfo();

		
		//}
		for (auto itr = _house.begin(); itr != _house.end(); ++itr) {
			(*itr)->Render();
		}

		for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
			(*itr)->Render();
		}

		for (auto itr = _uObj.begin(); itr != _uObj.end(); ++itr) {
			(*itr)->Render();
		}
	}


	if (_drawDebug) {
		_player->DrawDebugInfo();
		for (auto itr = _house.begin(); itr != _house.end(); ++itr) {
			(*itr)->DrawDebugInfo();
		}
		for (auto itr = _tower.begin(); itr != _tower.end(); ++itr) {
			(*itr)->DrawDebugInfo();
		}
		for (auto itr = _uObj.begin(); itr != _uObj.end(); ++itr) {
			(*itr)->DrawDebugInfo();
		}
	}

	if (_gate != nullptr) {
		_gate->Draw();
	}
	
	SetUseZBuffer3D(FALSE);

	_effectManeger->Render();


	//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	for (int i = 0; i < sizeof(ui) / sizeof(ui[0]); i++) {
		ui[i]->Draw();
	}

	if (_player->GetStaminaRate() < 1.0f) {
		int handleNum = floorf(_player->GetStaminaRate() * 100.0f / 33.4f);
		_gaugeUI[1]->Draw(_gaugeHandle[handleNum]);
		_gaugeUI[0]->Draw(_gaugeHandle[3]);
	}

	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(62);
	DrawFormatString(45, 200, GetColor(0, 0, 0), "%d", _player->GetInstance()->GetNowLevel() + 1);
	SetFontSize(16);
	//for (auto itr = _buildingBase.begin(); itr != _buildingBase.end(); ++itr) {
	//	(*itr)->DrawDebugInfo();
	//}

	return true;
}
