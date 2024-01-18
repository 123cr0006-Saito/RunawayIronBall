#include "UIBase.h"
UIBase::UIBase(VECTOR pos, std::string handleName) {
	_handle = new int[1];
	*_handle = ResourceServer::LoadGraph(handleName.c_str());
	_pos = pos;
	_handleNum = 0;
	int _x, _y;
	GetGraphSize(_handle[_handleNum], &_x, &_y);
	_cx = _x / 2;
	_cy = _y / 2;
};

UIBase::UIBase(VECTOR pos, std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf) {
	_handle = new int[AllNum];
	ResourceServer::LoadDivGraph(handleName.c_str(), AllNum, XNum, YNum, XSize, YSize, _handle);
	_pos = pos;
	_handleNum = 0;
	int _x, _y;
	GetGraphSize(_handle[_handleNum], &_x, &_y);
	_cx = _x / 2;
	_cy = _y / 2;
};

UIBase::~UIBase() {
	//“Á‚É‚È‚µ
};

bool UIBase::Process() {
	return true;
};

bool UIBase::Draw() {
	int _cx, _cy;
	GetGraphSize(_handle[0], &_cx, &_cy);
	static_cast<float> (_cx, _cy);
	_cx = _cx /= 2;
	_cy = _cy /= 2;

	DrawGraphF(_pos.x - _cx, _pos.y - _cy, _handle[_handleNum], true);
	return true;
};