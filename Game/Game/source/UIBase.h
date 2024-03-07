#pragma once
#include "appframe.h"
#include "dxlib.h"
#include <string>
//��ʂɕ\������UI�̊�ՂƂȂ�N���X
class UIBase
{
public:

	UIBase(VECTOR pos, std::string handleName);
	UIBase(VECTOR pos, std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf);
	UIBase(VECTOR pos, int size, int* handle);
	virtual ~UIBase();

	virtual bool Process();
	virtual bool Draw();

protected:
	int* _handle;
	int _handleNum;
	VECTOR _pos;
	float _cx, _cy;
};
