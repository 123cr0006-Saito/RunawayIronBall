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
	VERTEX2D _residue[4];//2d�|���S���Ŏg�����_�f�[�^ �c��̃Q�[�W����
	VERTEX2D _suppression[4];//2d�|���S���Ŏg�����_�f�[�^ ��������

	std::pair<VECTOR, int> _ironHandleSize; // first ���W second ���a

	static const unsigned short vertex[6];//���_�f�[�^���ȗ������邽�߂̔z��
};