#include "UIBase.h"
UIBase::UIBase(std::string handleName) {
	_handle = new int[1];
	*_handle = global.resource->LoadGraph(handleName.c_str());
	_pos = VGet(0, 0, 0);
	_imageNum = 0;
};

UIBase::UIBase(std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf) {
	_handle = new int[AllNum];
	global.resource->LoadDivGraph(handleName.c_str(),AllNum,XNum,YNum,XSize,YSize,_handle);
	 _pos = VGet(0,0,0);
	_imageNum = 0;
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

	DrawGraphF(_pos.x - _cx, _pos.y - _cy, _handle[_imageNum], true);
	return true;
};