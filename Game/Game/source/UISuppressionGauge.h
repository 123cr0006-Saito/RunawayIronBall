#pragma once
#include "UIBase.h"
#include "EnemyPool.h"
class UISuppressionGauge : public UIBase
{
public : 
	UISuppressionGauge(VECTOR pos, int size, int* handle);
	~UISuppressionGauge();
	virtual bool Process()override;
	virtual bool Draw()override;
protected:
	VERTEX2D _residue[4];//2dポリゴンで使う頂点データ 残りのゲージ部分
	VERTEX2D _suppression[4];//2dポリゴンで使う頂点データ 制圧部分

	std::pair<VECTOR, int> _ironHandleSize; // first 座標 second 半径

	static const unsigned short vertex[6];//頂点データを簡略化するための配列
};