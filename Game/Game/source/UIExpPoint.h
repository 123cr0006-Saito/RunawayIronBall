#pragma once
#include "../AppFrame/source/System/Header/Resource/ResourceServer.h"
#include "UIBase.h"
#include "player.h"
class UIExpPoint : public UIBase
{
public:
	UIExpPoint(VECTOR pos);
	~UIExpPoint()override;
	void SetRatio();
	virtual bool Process()override;
	virtual bool Draw()override;
protected:
	VERTEX2D _front[4];//2dポリゴンで使う頂点データ 本体

	static const int _levelMax = 10;
	int _currentTime;
	float _ratio;
	float _nowRatio, _nextRatio;

	int _easingframe;

	Player* _player;
	int _levelHandle[_levelMax];
	int flontHandleX, flontHandleY;

	static const unsigned short vertex[6];//頂点データを簡略化するための配列
};

