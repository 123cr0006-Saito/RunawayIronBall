#include "Fog.h"
#include "modegame.h"
Fog::Fog(){
	_player = Player::GetInstance();
	SetFogColor(255,255,255);
	_fogEndDistance = _fogDistanceMax;
	_param = _fogDistanceMax;
	_easingCount = 0;
	IsFog= false;
};
Fog::~Fog(){
	_player = nullptr;
	if (IsFog) {
		SetFogEnable(false);
	}
};

void Fog::UpdateIsFog(int stageNum){
	
	VECTOR pos = _player->GetPosition();
	float length = VSquareSize(pos);

	float stage_length[3] = { STAGE_ONE_WIDTH ,STAGE_TWO_WIDTH,STAGE_THREE_WIDTH };
	float fogLength = stage_length[stageNum-1] - 400.0f;
	if (length > fogLength * fogLength) {
		if(IsFog == false){
		IsFog = true;
		SetFogEnable(IsFog);
		_fogEndDistance = _fogDistanceMin;
		_easingCount = 0;
		}
	}
	else {
		if(IsFog == true){
		   IsFog = false;
		   _fogEndDistance = _fogDistanceMax;
		   _easingCount = 0;
		}
	}
};

void Fog::Process(int stageNum){
	
	UpdateIsFog(stageNum);
	int easingTime = 60;

	if(_easingCount < easingTime){
		_easingCount++;
		_param = Easing::Linear(_easingCount, _param, _fogEndDistance, easingTime);
		SetFogStartEnd(1000, _param);
		if(!IsFog && _easingCount >= easingTime){
			SetFogEnable(IsFog);
		}
	}

};