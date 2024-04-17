//----------------------------------------------------------------------
// @filename UIHeart.h
// ＠date: 2023/12/25
// ＠author: saito ko
// @explanation
// プレイヤーHPのUIを表示するクラス
//----------------------------------------------------------------------
#pragma once
#include "UIBase.h"
#include "Player.h"

class UIHeart : public UIBase
{
public:
	UIHeart(VECTOR pos, std::string handleName);
	UIHeart(VECTOR pos, std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf);
	UIHeart(VECTOR pos, int size, int* handle,int damageHandleNum);

	~UIHeart()override;

	void SetDamage(int hp);
	virtual bool  Process()override;
	virtual bool  Draw()override;


private:
	DrawGauge* _heart;// ハートの画像
	Player* _player;// プレイヤークラス
	int _oldHp;// 前回のHP
	bool _IsDamage;// ダメージを受けたかどうか
	int _currentTime;// 現在の時間

	static const int _damageSeconds;// ダメージを受けている時間
};

