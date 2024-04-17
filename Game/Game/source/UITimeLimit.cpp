//----------------------------------------------------------------------
// @filename UITimeLimit.cpp
// ＠date: 2024/03/11
// ＠author: saito ko
// @explanation
// 残り時間を表示するUIクラス
//----------------------------------------------------------------------
#include "UITimeLimit.h"
//----------------------------------------------------------------------
// @brief コンストラクタ
// @param pos 位置
// ＠return 無し
//----------------------------------------------------------------------
UITimeLimit::UITimeLimit(VECTOR pos) :UIBase(pos){
	_timeLimit = TimeLimit::GetInstance();
	_numPos = VAdd(pos,VGet(200,130,0));

	_handle = NEW int[10];
	ResourceServer::LoadMultGraph("TimeNum","res/UI/Time/Ui_Time",".png",10,_handle);
	_colonHandle = ResourceServer::Load("TimeColon", "res/UI/Time/UI_Time_Colon.png");
	_frameHandle = ResourceServer::Load("TimeFrame", "res/UI/Time/UI_TIME_Gauge.png");
};
//----------------------------------------------------------------------
// @brief デストラクタ
// ＠return 無し
//----------------------------------------------------------------------
UITimeLimit::~UITimeLimit(){
	_timeLimit = nullptr;
	if (_handle != nullptr) {
		delete[] _handle; _handle = nullptr;
	}
};
//----------------------------------------------------------------------
// @brief 更新処理
// ＠return 成功しているか
//----------------------------------------------------------------------
bool UITimeLimit::Process(){
	return true;
};
//----------------------------------------------------------------------
// @brief 描画処理
// ＠return 成功しているか
//----------------------------------------------------------------------
bool UITimeLimit::Draw() {
	DrawGraph(_pos.x, _pos.y, _frameHandle, true);
	int time = _timeLimit->GetTimeLimit();
	int loopCount = 0;

	int x = (int)_numPos.x;
	while (1) {

		int handleX, handleY;
		if (loopCount == 2) {
			// コロンを描画
			GetGraphSize(_colonHandle, &handleX, &handleY);
			DrawGraph(x+ handleX, _numPos.y , _colonHandle, true);
			x -= handleX+20;// 数字の間隔
		}
		
		// 秒数を描画
		int num = time % 10;
		DrawGraph(x, _numPos.y, _handle[num], true);
		time /= 10;

		GetGraphSize(_handle[num], &handleX, &handleY);

		x -= handleX+10;// 数字の間隔
		loopCount++;// 何桁目か数える

		if (time == 0) {
			break;
		}

	}
	return true;
};