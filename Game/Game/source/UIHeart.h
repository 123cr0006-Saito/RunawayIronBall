#pragma once
#include "UIBase.h"
#include "Player.h"

class UIHeart : public UIBase
{
public:

	UIHeart(VECTOR pos, std::string handleName);
	UIHeart(VECTOR pos, std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf);
	UIHeart(VECTOR pos, int size, int* handle,int damageHandleNum);

	~UIHeart();

	void SetDamage();
	virtual bool  Process()override;
	virtual bool  Draw()override;

	//�f�o�b�O�p
	void DecreaseHP() { _hp--; };

private:
	DrawGauge* _heart;
	int _oldHp;
	bool _IsDamage;
	int _currentTime;

	static const int _damageSeconds;
	static const int _maxHp;
	//--------------------------------
	//��ŏ����܂�
	int _hp;//�v���C���[�N���X�ł������ł���܂ł̉�
};

