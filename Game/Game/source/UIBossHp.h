//----------------------------------------------------------------------
// @filename UIBossHp.h
// @date: 2024/03/21
// @author: saito ko
// @explanation
// ボスのHPを表示するUIクラス
//----------------------------------------------------------------------
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
	int _currentTime;// 現在の時間
	float _ratio;// 現在の割合
	float _nowRatio, _nextRatio;// 現在の割合と次の割合
	int _easingframe;// イージングのフレーム
	int flontHandleX, flontHandleY;// bossHPバーのハンドルの横幅と縦幅
	static const unsigned short vertex[6];//頂点データを簡略化するための配列
};

