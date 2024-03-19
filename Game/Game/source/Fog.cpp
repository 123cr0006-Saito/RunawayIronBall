#include "Fog.h"
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

void Fog::UpdateIsFog(){
	

	VECTOR pos = _player->GetPosition();
	float length = VSquareSize(pos);
	float fogLength = 27000 / 2.0f;
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

void Fog::Process(){
	
	UpdateIsFog();
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