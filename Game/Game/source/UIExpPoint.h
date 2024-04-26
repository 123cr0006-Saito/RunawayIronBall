//----------------------------------------------------------------------
// @filename UIExpPoint.cpp
// @date: 2024/12/25
// @author: saito ko
// @explanation
// 経験値のUIを表示するクラス
//----------------------------------------------------------------------
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

	static const int _levelMax = 10;// レベルの最大数
	int _currentTime;// 現在の時間
	float _ratio;// 経験値の割合
	float _nowRatio, _nextRatio;// 現在の割合と次の割合

	int _easingframe;// イージングのフレーム

	Player* _player;// プレイヤークラス
	int _levelHandle[_levelMax];// レベルの画像ハンドル
	int flontHandleX, flontHandleY;// 経験値バーの画像ハンドルの横幅と縦幅

	static const unsigned short vertex[6];//頂点データを簡略化するための配列
};

