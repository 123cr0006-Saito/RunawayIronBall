#include "UIBossHp.h"

const unsigned short UIBossHp::vertex[6]{ 0,1,2,2,1,3 };

UIBossHp::UIBossHp(VECTOR pos) : UIBase(pos){
	_handle = new int[2];
	std::string path = "res/UI/BossGauge/";
	std::string name[2] = { "UI_EXP_Gauge_Black","UI_EXP_Gauge_Red" };
	for (int i = 0; i < 2; i++) {
		_handle[i] = ResourceServer::Load(name[i], path + name[i] + ".png");
	}

	_ratio = 0.0f;
	_nowRatio = 0.0f;
	_nextRatio = 0.0f;

	// 下にあるフレームからゲージずれる差分
	int flontHandleSizeX = 136;
	int flontHandleSizeY = 37;

	GetGraphSize(_handle[1], &flontHandleX, &flontHandleY);

	float posTbl[][4] = {
		{flontHandleSizeX,flontHandleSizeY,0.0f,0.0f},
		{flontHandleSizeX + flontHandleX,flontHandleSizeY,1.0f,0.0f},
		{flontHandleSizeX,flontHandleSizeY + flontHandleY,0.0f,1.0f},
		{flontHandleSizeX + flontHandleX,flontHandleSizeY + flontHandleY,1.0f,1.0f}
	};

	for (int i = 0; i < 4; i++) {
		_vertex[i].pos = VAdd(_pos, VGet(posTbl[i][0], posTbl[i][1], 0));
		_vertex[i].u = posTbl[i][2];
		_vertex[i].v = posTbl[i][3];
		_vertex[i].dif = GetColorU8(255, 255, 255, 255);
		_vertex[i].rhw = 1.0f;
	}
};

UIBossHp::~UIBossHp(){
	if (_handle != nullptr) {
		delete[] _handle; _handle = nullptr;
	}
};

void UIBossHp::SetRatio(int nowHp,int maxHp){
	static int oldHp;
	
	int easingTime = 30.0f / 60.0f * 1000;
	int nowTime = GetNowCount() - _currentTime;

	if (oldHp != nowHp) {
		_nextRatio = (float)nowHp / maxHp;
		_nowRatio = _ratio;
		_currentTime = GetNowCount();
	}

	if (nowTime <= easingTime) {
		_ratio = Easing::OutSine(nowTime, _nowRatio, _nextRatio, easingTime);
	}

	oldHp = nowHp;
};

bool UIBossHp::Process(int nowHp, int maxHp){

	SetRatio(nowHp,maxHp);

	for (int i = 0; i < 2; i++) {
		_vertex[i * 2].pos.x = _vertex[1].pos.x - flontHandleX * (1 - _ratio);
		_vertex[i * 2].u = _ratio;
	}

	return true;
};

bool UIBossHp::Draw(){
	// 経験値フレーム
	DrawGraph(_pos.x, _pos.y, _handle[0], true);

	// 経験値バー
	DrawPrimitiveIndexed2D(_vertex, 4, vertex, 6, DX_PRIMTYPE_TRIANGLELIST, _handle[1], true);

	return true;
};