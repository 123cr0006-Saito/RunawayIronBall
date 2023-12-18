#pragma once
#include "EnemyBase.h"
#include "Player.h"
#include <string>
class SlaBlock : public EnemyBase
{
public:
	SlaBlock(int model, VECTOR pos, Player* player);
	~SlaBlock();

	virtual bool Process() override;
	virtual bool Render() override;

protected:
	static const float _fixSartchSize;//õ“G”ÍˆÍ‚Ì”¼Œa
	static const float _fixDiscoverSize;//”­Œ©A‘ÎÛ‚ÌŒ©¸‚¤‚Ü‚Å‚Ì‹——£‚Ì”¼Œa
	static const float _fixAttackSize;//”­Œ©AUŒ‚ƒ‚[ƒVƒ‡ƒ“‚É“ü‚é‚Ü‚Å‚Ì”ÍˆÍ
};