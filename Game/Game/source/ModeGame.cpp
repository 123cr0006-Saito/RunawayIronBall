#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeGameOver.h"

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }


	_camera = new Camera();

	_skySphere = MV1LoadModel(_T("res/SkySphere/skysphere.mv1"));
	MV1SetPosition(_skySphere, VGet(0, 0, 0));
	MV1SetScale(_skySphere, VGet(1,1,1));

	int playerModelHandle = MV1LoadModel("res/Character/cg_player_girl/cg_player_girl.mv1");
	_player = new Player(playerModelHandle, VGet(0,0,0));

	_chain = new Chain();
	_chain->Init();


	int enemyModelHandle = MV1LoadModel("res/Character/enemy/cg_enemy_float.mv1");

	for (int i = 0; i < 10; i++) {
		VECTOR vTmp = VGet((200.0f * (i+ 1)) + 1000.0f, 0, 0);
		vTmp = VTransform(vTmp, MGetRotY(float(i + 1) * Math::DegToRad(20.0f)));
		_enemy[i] = new Enemy(MV1DuplicateModel(enemyModelHandle), vTmp);
	}

	_seHandle = LoadSoundMem("res/se_rbenemy_break.mp3");

	return true;
}

bool ModeGame::Terminate() {
	base::Terminate();
	return true;
}

bool ModeGame::Process() {
	base::Process();
	
	_player->Process(_camera->GetCamY());
	_chain->Process(_player->GetRightHandPos());

	for (int i = 0; i < 10; i++) {
		_enemy[i]->Process(_player->GetPosition());
	}

	
	for (int i = 0; i < 10; i++) {
		if (Collision3D::SphereCol(_enemy[i]->GetPosition(), 160.0f, _chain->GetBallPosition(), 130.0f)) {
			if (_chain->GetIsSwing()) {
				if (_enemy[i]->GetIsHit() == false) {
					VECTOR vDir = VSub(_enemy[i]->GetPosition(), _player->GetPosition());
					vDir.y = 200.0f;
					vDir = VNorm(vDir);
					_enemy[i]->SetIsHit(vDir);
					PlaySoundMem(_seHandle, DX_PLAYTYPE_BACK);
				}
			}
			else {
				VECTOR vDir = VSub(_enemy[i]->GetPosition(), _chain->GetBallPosition());
				vDir = VNorm(vDir);
				VECTOR pos = VAdd(_chain->GetBallPosition(), VScale(vDir, 160.0f + 130.0));
				_enemy[i]->SetPos(pos);
			}

		}
	}

	_camera->Process(_player->GetPosition());
	return true;
}

bool ModeGame::Render() {
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	// ƒ‰ƒCƒgÝ’è
	SetUseLighting(TRUE);
	//clsDx();
	
	MV1DrawModel(_skySphere);
	// 0,0,0‚ð’†S‚Éü‚ðˆø‚­
	{
		float linelength = 1000.f;
		VECTOR v = { 0, 0, 0 };
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}
		
	_player->Render();
	_chain->Render();

	for (int i = 0; i < 10; i++) {
		_enemy[i]->Render();

		VECTOR ePos = _enemy[i]->GetPosition();
		DrawSphere3D(ePos, 160.0f, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
	}

	VECTOR ballPos = _chain->GetBallPosition();
	DrawSphere3D(ballPos, 130.0f, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), false);



	SetUseZBuffer3D(FALSE);
	return true;
}

