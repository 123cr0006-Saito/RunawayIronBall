#pragma once
#include "appframe.h"
#include "dxlib.h"
#include <string>
//画面に表示するUIの基盤となるクラス
class UIBase
{
	UIBase(std::string handleName);
	UIBase(std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf);
	~UIBase();

	virtual bool Process();
	virtual bool Draw();

protected:
	int* _handle;
	int _imageNum;
	VECTOR _pos;
};

