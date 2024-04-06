//----------------------------------------------------------------------
// @filename UIHeart.h
// ��date: 2023/12/25
// ��author: saito ko
// @explanation
// �v���C���[HP��UI��\������N���X
//----------------------------------------------------------------------
#pragma once
#include "UIBase.h"
#include "Player.h"

class UIHeart : public UIBase
{
public:
	UIHeart(VECTOR pos, std::string handleName);
	UIHeart(VECTOR pos, std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf);
	UIHeart(VECTOR pos, int size, int* handle,int damageHandleNum);

	~UIHeart()override;

	void SetDamage(int hp);
	virtual bool  Process()override;
	virtual bool  Draw()override;


private:
	DrawGauge* _heart;// �n�[�g�̉摜
	Player* _player;// �v���C���[�N���X
	int _oldHp;// �O���HP
	bool _IsDamage;// �_���[�W���󂯂����ǂ���
	int _currentTime;// ���݂̎���

	static const int _damageSeconds;// �_���[�W���󂯂Ă��鎞��
};

