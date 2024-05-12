//----------------------------------------------------------------------
// @filename UIExpPoint.cpp
// @date: 2024/12/25
// @author: saito ko
// @explanation
// �o���l��UI��\������N���X
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
	VERTEX2D _front[4];//2d�|���S���Ŏg�����_�f�[�^ �{��

	static const int _levelMax = 10;// ���x���̍ő吔
	int _currentTime;// ���݂̎���
	float _ratio;// �o���l�̊���
	float _nowRatio, _nextRatio;// ���݂̊����Ǝ��̊���

	int _easingframe;// �C�[�W���O�̃t���[��

	Player* _player;// �v���C���[�N���X
	int _levelHandle[_levelMax];// ���x���̉摜�n���h��
	int flontHandleX, flontHandleY;// �o���l�o�[�̉摜�n���h���̉����Əc��

	static const unsigned short vertex[6];//���_�f�[�^���ȗ������邽�߂̔z��
};

