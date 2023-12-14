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
	static const float _sartchRangeSize;//õ“G”ÍˆÍ‚Ì”¼Œa
	static const float _discoverRangeSize;//”­Œ©A‘ÎÛ‚ÌŒ©¸‚¤‚Ü‚Å‚Ì‹——£‚Ì”¼Œa

	//‰ñù‚³‚¹‚é‚½‚ß‚Ì•Ï”
	float _nextDir;
	float _oldDir;
};

