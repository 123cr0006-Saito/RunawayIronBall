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

	//デバッグ用
	//void DecreaseHP() { _hp--; };

private:
	DrawGauge* _heart;
	Player* _player;
	int _oldHp;
	bool _IsDamage;
	int _currentTime;

	static const int _damageSeconds;
};

