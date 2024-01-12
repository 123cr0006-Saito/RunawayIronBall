#pragma once
#include "UIBase.h"
#include "Player.h"

class UIHeart : public UIBase
{
public:

	UIHeart(VECTOR pos, std::string handleName);
	UIHeart(VECTOR pos, std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf);

	~UIHeart();

	virtual bool  Process()override;
	virtual bool  Draw()override;

private:
	//--------------------------------
	//後で消します
	int _hp;//プレイヤークラスでｈｐができるまでの仮
};

