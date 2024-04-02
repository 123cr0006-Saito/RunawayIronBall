#include "UIBase.h"

UIBase::UIBase(VECTOR pos) {
	_pos = pos;
	_handleNum = 0;
	_cx = 0;
	_cy = 0;
};

UIBase::UIBase(VECTOR pos, std::string handleName) {
	_handle = NEW int[1];
	*_handle = ResourceServer::LoadGraph(handleName.c_str(),handleName.c_str());
	_pos = pos;
	_handleNum = 0;
	float _x, _y;
	GetGraphSizeF(_handle[_handleNum], &_x, &_y);
	_cx = _x / 2;
	_cy = _y / 2;
};

UIBase::UIBase(VECTOR pos, std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf) {
	_handle = NEW int[AllNum];
	ResourceServer::LoadDivGraph(handleName.c_str(), handleName.c_str(), AllNum, XNum, YNum, XSize, YSize, _handle);
	_pos = pos;
	_handleNum = 0;
	float _x, _y;
	GetGraphSizeF(_handle[_handleNum], &_x, &_y);
	_cx = _x / 2;
	_cy = _y / 2;
};

UIBase::UIBase(VECTOR pos, int size, int* handle) {
	_handle = NEW int[size];
	for (int i = 0; i < size; i++) {
		_handle[i] = handle[i];
	}
	_pos = pos;
	_handleNum = 0;
	float _x, _y;
	GetGraphSizeF(_handle[_handleNum], &_x, &_y);
	_cx = _x / 2;
	_cy = _y / 2;
};

UIBase::~UIBase() {
	if (_handle != nullptr) {
		delete[] _handle; _handle = nullptr;
	}
};

bool UIBase::Process() {
	return true;
};

bool UIBase::Draw() {
	float _cx, _cy;
	GetGraphSizeF(_handle[0], &_cx, &_cy);
	
	_cx = _cx /= 2;
	_cy = _cy /= 2;

	DrawGraphF(_pos.x - _cx, _pos.y - _cy, _handle[_handleNum], true);
	return true;
};