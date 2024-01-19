#pragma once
#include "appframe.h"
#include "myJson.h"
#include <vector>
#include <string>

//始まる位置　次の位置・・・

class LoadSlideUI {
public:
	LoadSlideUI(std::string handleName, std::string JsonFileName);
	~LoadSlideUI();
	virtual bool Process();
	virtual bool Draw();

	bool GetUse() { return _IsUse; }
protected:

	typedef struct {
		int _nextX, _nextY;
		int _travelTime;
		int _waitingTime;
	}Param;

	Param* _value;
	int _size;
	int _moveNum;
	int _x, _y;
	int _handle;
	int _currentTime;
	bool _IsUse;
};

//初めにSlideUIクラスを作成しないとエラーが起きます。
class SlideUI
{
public:
	SlideUI() { instance = this; };

	virtual bool Process();
	virtual bool Draw();

	static SlideUI* instance;
	static SlideUI* GetInstance() { return  instance; }

	std::vector<LoadSlideUI*> _UIList;
};