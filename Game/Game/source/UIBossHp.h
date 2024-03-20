#pragma once
#include "UIBase.h"
#include "../AppFrame/source/System/Header/Resource/ResourceServer.h"
class UIBossHp : public UIBase
{
public:
	UIBossHp(VECTOR pos);
	~UIBossHp()override;
	void SetRatio(int nowHp, int maxHp);
	bool Process(int nowHp, int maxHp);
	virtual bool Draw()override;
protected:
	VERTEX2D _vertex[4];//2dポリゴンで使う頂点データ 本体

	int _currentTime;
	float _ratio;
	float _nowRatio, _nextRatio;

	int _easingframe;

	int flontHandleX, flontHandleY;

	static const unsigned short vertex[6];//頂点データを簡略化するための配列
};

