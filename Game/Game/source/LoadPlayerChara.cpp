#include "LoadPlayerChara.h"
LoadPlayerChara::LoadPlayerChara(){

	//_modelHandle = ResourceServer::Load("Player", "res/Character/cg_player_girl/cg_player_girl_TEST_Ver.2.mv1");
	_modelHandle = MV1LoadModel("res/Character/cg_player_girl/cg_player_girl_TEST_Ver.2.mv1");
	float rotation = Math::DegToRad(90);// -x •ûŒü‚ÉŒü‚¯‚é
	MV1SetRotationXYZ(_modelHandle, VGet(0,rotation,0));

	_pos = VGet(0, 0, 0);
	
	// “S‹…‚Ì‰Šú‰»
	_chain = new Chain();
	_chain->Init();
	_chain->SetPlayerModelHandle(_modelHandle);
	// “S‹…‚ÌˆÚ“®ó‘Ô‚ğu’Ç]v‚Éİ’è
	_chain->SetMoveState(IB_MOVE_STATE::FOLLOWING);
	_chain->SetBallPosition(VAdd(_pos, VGet(500, 0, 0)));

	// F‚Ì’²®
	_modelColor = new ModelColor();
	_modelColor->Init(_modelHandle);
	_modelColor->ChangeFlickerTexture(true);

	//ƒAƒjƒ[ƒVƒ‡ƒ“‚Ì‰Šú‰»
	std::string frameName;
	if (rand() % 2 == 0) {
		frameName = "MO_PL_Walk";
	}
	else {
		frameName = "MO_PL_Run";
	}

	int animIIdex = MV1GetAnimIndex(_modelHandle, frameName.c_str());

	_attachIndex = MV1AttachAnim(_modelHandle, animIIdex, -1, false);
	_totalTime = MV1GetAttachAnimTotalTime(_modelHandle,_attachIndex);
	_playTime = 0.0f;
};

LoadPlayerChara::~LoadPlayerChara() {
	MV1DeleteModel(_modelHandle);
};

bool LoadPlayerChara::Process(){
	_chain->Process();

	_pos.x -= 20;
	MV1SetPosition(_modelHandle, _pos);
	AnimationProcess();
	return true;
};

bool LoadPlayerChara::AnimationProcess(){

	MV1SetAttachAnimTime(_modelHandle, _attachIndex, _playTime);

	_playTime += 1.0f;
	if (_playTime > _totalTime) {
		_playTime = 0.0f;
	}

	return true;
};

bool LoadPlayerChara::Render() {
	MV1DrawModel(_modelHandle);
	_chain->Render();
	return true;
};