#pragma once
#include "appframe.h"
#include "myJson.h"
#include <vector>
#include <string>

//�n�܂�ʒu�@���̈ʒu�E�E�E

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

//���߂�SlideUI�N���X���쐬���Ȃ��ƃG���[���N���܂��B
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