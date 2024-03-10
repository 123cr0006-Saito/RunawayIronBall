#include "UIExpPoint.h"

const unsigned short UIExpPoint::vertex[6]{ 0,1,2,2,1,3 };

UIExpPoint::UIExpPoint(VECTOR pos) :
	UIBase(pos) 
{
	_player = Player::GetInstance();
	_handle = new int[2];
	std::string path = "res/UI/UIGauge/";
	std::string name[2] = {"UI_EXP_Gauge_Black","UI_EXP_Gauge_Red"};
	for (int i = 0; i < 2; i++) {
		_handle[i] = ResourceServer::Load(name[i], path + name[i] + ".png");
	}
	ResourceServer::LoadMultGraph("UILevel", "res/UI/UILevel/UI_Level",".png",_levelMax, _levelHandle);

	_ratio = 0.0f;
	_nowRatio = 0.0f;
	_nextRatio = 0.0f;

	// 下にあるフレームからゲージずれる差分
	int flontHandleSizeX = 136;
	int flontHandleSizeY = 35;

	GetGraphSize(_handle[1], &flontHandleX, &flontHandleY);

	float posTbl[][4] = {
		{flontHandleSizeX,flontHandleSizeY,0.0f,0.0f},
		{flontHandleSizeX + flontHandleX,flontHandleSizeY,1.0f,0.0f},
		{flontHandleSizeX,flontHandleSizeY + flontHandleY,0.0f,1.0f},
		{flontHandleSizeX + flontHandleX,flontHandleSizeY + flontHandleY,1.0f,1.0f}
	};

	for (int i = 0; i < 4; i++) {
		_front[i].pos = VAdd(_pos,VGet(posTbl[i][0], posTbl[i][1], 0));
		_front[i].u = posTbl[i][2];
		_front[i].v = posTbl[i][3];
		_front[i].dif = GetColorU8(255, 255, 255, 255);
		_front[i].rhw = 1.0f;
	}

};

UIExpPoint::~UIExpPoint() {
	_player = nullptr;
};

void UIExpPoint::SetRatio() {
	static int oldExp;
	static int oldLevel;

	int easingTime = 30.0f / 60.0f * 1000;
	int nowTime = GetNowCount() - _currentTime;

	int nowExp = _player->GetNowExp();
	int nextExp = _player->GetNextExp();

	if (nowExp != oldExp) {
		int nowLevel = _player->GetNowLevel();
		_nextRatio = (float)nowExp / nextExp;
		if (nowLevel != oldLevel) {
			_nextRatio = 1 + _nextRatio;
		}
		_nowRatio = _ratio;
		_currentTime = GetNowCount();
		oldLevel = nowLevel;
	}

	if (nowTime <= easingTime) {
		if (_ratio >= 1.0f) {
			_nowRatio = 0.0f;
			_nextRatio -= 1.0f;
		}
		_ratio = Easing::OutSine(nowTime, _nowRatio, _nextRatio, easingTime);
	}

	oldExp = nowExp;
};

bool UIExpPoint::Process() {

	SetRatio();

	for (int i = 0; i < 2; i++) {
		_front[i*2 + 1].pos.x = _front[0].pos.x  + flontHandleX* _ratio;
		_front[i*2 + 1].u =  _ratio;
	}

	return true;
};

bool UIExpPoint::Draw() {

	// 経験値フレーム
	DrawGraph(_pos.x, _pos.y, _handle[0], true);

	// 経験値バー
	DrawPrimitiveIndexed2D(_front, 4, vertex, 6, DX_PRIMTYPE_TRIANGLELIST, _handle[1], true);

	// 現在のレベル表示
	int nowLevel = _player->GetNowLevel();
	int numX = 60; int numY = 80;
	DrawGraph(_pos.x + numX,_pos.y + numY, _levelHandle[nowLevel],true);
	return true;
};