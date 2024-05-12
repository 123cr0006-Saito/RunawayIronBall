//----------------------------------------------------------------------
// @filename Fog.h
// @date: 2024/02/22
// @author: saito ko
// @explanation
// プレイヤーが決められた範囲外に出るとフォグが発生するクラス
//----------------------------------------------------------------------
#pragma once
#include "dxlib.h"
#include "player.h"

class Fog
{
	public:
	Fog();
	~Fog();
	void Process(int stageNum);
	void UpdateIsFog(int stageNum);
	bool GetIsFog() { return IsFog; }
protected:
	Player* _player;//	プレイヤークラス
	bool IsFog;//フォグが発生しているかどうか
	const int _fogDistanceMax = 30000;//フォグが発生する最大距離
	const int _fogDistanceMin = 2000;//フォグが発生する最小距離
	float _fogEndDistance;//フォグが発生する距離
	int _param;//現在のフォグの濃さ
	int _easingCount;//イージングのカウント
};

