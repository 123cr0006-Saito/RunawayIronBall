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
	static const float _sartchRangeSize;//���G�͈͂̔��a
	static const float _discoverRangeSize;//�������A�Ώۂ̌������܂ł̋����̔��a

	//��������邽�߂̕ϐ�
	float _nextDir;
	float _oldDir;

	//�C�[�W���O�Ɏg�p
	int _easingFrame;
	VECTOR _saveNextPoint;
};

