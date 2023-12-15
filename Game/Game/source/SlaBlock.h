#pragma once
#include "EnemyBase.h"
#include "Player.h"
#include <string>
class SlaBlock :  public EnemyBase
{
public:
	SlaBlock(std::string model_name,VECTOR pos, Player* player);
	~SlaBlock();

	virtual bool Process() override;
	virtual bool Render() override;

protected:
	Player*  _player;
	static const float _sartchRangeSize;//索敵範囲の半径
	static const float _discoverRangeSize;//発見時、対象の見失うまでの距離の半径

	//回旋させるための変数
	float _nextDir;
	float _oldDir;

	//イージングに使用
	int _easingFrame;
	VECTOR _saveNextPoint;
};

