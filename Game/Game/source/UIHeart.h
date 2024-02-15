#pragma once
#include "UIBase.h"
#include "Player.h"

class UIHeart : public UIBase
{
public:

	UIHeart(VECTOR pos, std::string handleName);
	UIHeart(VECTOR pos, std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf);
	UIHeart(VECTOR pos, int size, int* handle,int damageHandleNum);

	~UIHeart();

	void SetDamage();
	virtual bool  Process()override;
	virtual bool  Draw()override;

	//デバッグ用
	void DecreaseHP() { _hp--; };

private:
	DrawGauge* _heart;
	int _oldHp;
	bool _IsDamage;
	int _currentTime;

	static const int _damageSeconds;
	static const int _maxHp;
	//--------------------------------
	//後で消します
	int _hp;//プレイヤークラスでｈｐができるまでの仮
};

