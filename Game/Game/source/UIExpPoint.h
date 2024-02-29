#pragma once
#include "UIBase.h"
#include "player.h"
class UIExpPoint : public UIBase
{
public:
	UIExpPoint(VECTOR pos, std::string handleName);
	UIExpPoint(VECTOR pos, std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf);
	UIExpPoint(VECTOR pos, int size, int* handle);
	~UIExpPoint();
	virtual bool Process()override;
	virtual bool Draw()override;
protected:
	VERTEX2D _back[4];//2dポリゴンで使う頂点データ 背景
	VERTEX2D _front[4];//2dポリゴンで使う頂点データ 本体

	Player* _player;

	static const unsigned short vertex[6];//頂点データを簡略化するための配列
};

