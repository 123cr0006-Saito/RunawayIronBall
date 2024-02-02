#pragma once
#include "UIBase.h"
#include "Player.h"

class UIHeart : public UIBase
{
public:

	UIHeart(VECTOR pos, std::string handleName);
	UIHeart(VECTOR pos, std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf);

	~UIHeart()override;

	void SetDamage();
	virtual bool  Process()override;
	virtual bool  Draw()override;

private:
	DrawGauge* _heart;
	int _oldHp;
	bool _IsDamage;
	int _currentTime;

	static const int _damageSeconds;
	//--------------------------------
	//��ŏ����܂�
	int _hp;//�v���C���[�N���X�ł������ł���܂ł̉�
};

