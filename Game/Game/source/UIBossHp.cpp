//----------------------------------------------------------------------
// @filename UIBossHp.cpp
// @date: 2024/03/21
// @author: saito ko
// @explanation
// ボスのHPを表示するUIクラス
//----------------------------------------------------------------------
#include "UIBossHp.h"

const unsigned short UIBossHp::vertex[6]{ 0,1,2,2,1,3 };
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param pos 位置
// @return 無し
//----------------------------------------------------------------------
UIBossHp::UIBossHp(VECTOR pos) : UIBase(pos){
	_handle = NEW int[2];
	std::string path = "res/UI/Boss/";
	std::string name[2] = { "UI_Boss_Hp_Frame","gauge" };
	for (int i = 0; i < 2; i++) {
		_handle[i] = ResourceServer::Load(name[i], path + name[i] + ".png");
	}

	_ratio = 0.0f;
	_nowRatio = 0.0f;
	_nextRatio = 0.0f;

	// 下にあるフレームからゲージずれる差分
	int flontHandleSizeX = 25;
	int flontHandleSizeY = 165;

	GetGraphSize(_handle[1], &flontHandleX, &flontHandleY);

	float posTbl[][4] = {
		{flontHandleSizeX,flontHandleSizeY,0.0f,0.0f},
		{flontHandleSizeX + flontHandleX,flontHandleSizeY,1.0f,0.0f},
		{flontHandleSizeX,flontHandleSizeY + flontHandleY,0.0f,1.0f},
		{flontHandleSizeX + flontHandleX,flontHandleSizeY + flontHandleY,1.0f,1.0f}
	};
	// 頂点の設定
	for (int i = 0; i < 4; i++) {
		_vertex[i].pos = VAdd(_pos, VGet(posTbl[i][0], posTbl[i][1], 0));
		_vertex[i].u = posTbl[i][2];
		_vertex[i].v = posTbl[i][3];
		_vertex[i].dif = GetColorU8(255, 255, 255, 255);
		_vertex[i].rhw = 1.0f;
	}
};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
UIBossHp::~UIBossHp(){
	if (_handle != nullptr) {
		delete[] _handle; _handle = nullptr;
	}
};
//----------------------------------------------------------------------
// @brief bossの最大HPから見た現在のHPの割合を計算
// @param nowHp 現在のHP
// @param maxHp 最大HP
// @return 無し
//----------------------------------------------------------------------
void UIBossHp::SetRatio(int nowHp,int maxHp){
	static int oldHp;
	
	int easingTime = 60.0f / 60.0f * 1000;
	int nowTime = GetNowCount() - _currentTime;

	if (oldHp != nowHp) {
		_nextRatio = (float)nowHp / maxHp;
		_nowRatio = _ratio;
		_currentTime = GetNowCount();
	}

	if (nowTime <= easingTime) {
		_ratio = Easing::OutSine(nowTime, _nowRatio, _nextRatio, easingTime);
		if(_ratio < 0){
			_ratio = 0;
		}
	}

	oldHp = nowHp;
};
//----------------------------------------------------------------------
// @brief 更新処理
// @param nowHp bossの現在のHP
// @param maxHp bossの最大HP
// @return 成功しているか
//----------------------------------------------------------------------
bool UIBossHp::Process(int nowHp, int maxHp){
	// HPの割合を計算
	SetRatio(nowHp,maxHp);
	// 頂点の設定
	for (int i = 0; i < 2; i++) {
		_vertex[i * 2].pos.x = _vertex[1].pos.x - flontHandleX * ( _ratio);
		_vertex[i * 2].u = 1-_ratio;
	}

	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// @return 成功しているか
//----------------------------------------------------------------------
bool UIBossHp::Draw(){
	// 経験値フレーム
	DrawGraph(_pos.x, _pos.y, _handle[0], true);

	// 経験値バー
	DrawPrimitiveIndexed2D(_vertex, 4, vertex, 6, DX_PRIMTYPE_TRIANGLELIST, _handle[1], true);

	return true;
};