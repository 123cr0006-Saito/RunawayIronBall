#pragma once
#include "UIBase.h"
class UIExpPoint : public UIBase
{
public:
	UIExpPoint(VECTOR pos, std::string handleName);
	UIExpPoint(VECTOR pos, std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf);
	~UIExpPoint();
	virtual bool Process()override;
	virtual bool Draw()override;
protected:
	VERTEX2D _back[4];//2d�|���S���Ŏg�����_�f�[�^ �w�i
	VERTEX2D _front[4];//2d�|���S���Ŏg�����_�f�[�^ �{��

	float a = 0.0f;

	static const unsigned short vertex[6];//���_�f�[�^���ȗ������邽�߂̔z��
};
