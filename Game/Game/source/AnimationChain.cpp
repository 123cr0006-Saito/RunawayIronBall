#include "AnimationChain.h"
AnimationChain::AnimationChain(VECTOR pos,float angle){
	_handle = ResourceServer::LoadGraph("UI_Chain","res/UI/Chain/Kusari_Straight.png");
	_frameCount = 0;
	_maxCount = 60;
	_endX = pos.x;
	_endY = pos.y;
	_angle = angle * DX_PI / 180;
	float distance = 10000;
	_startX = pos.x - cos(_angle) * distance;
	_startY = pos.y - sin(_angle) * distance;
	_nowX = _startX;
	_nowY = _startY;
};

AnimationChain::~AnimationChain(){

};

void AnimationChain::Process(){
	if (_frameCount < _maxCount) {
		_nowX = _startX + (_endX - _startX) * _frameCount / _maxCount;
		_nowY = _startY + (_endY - _startY) * _frameCount / _maxCount;
		_frameCount++;
	}
};

void AnimationChain::Draw(){
	DrawRotaGraph(_nowX, _nowY, 0.8f, _angle, _handle, true);
};