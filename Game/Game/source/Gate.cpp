#include "Gate.h"
Gate::Gate(VECTOR pos, int r, int* handle, int size, int animsPeed, int handlesize) :
_pos(pos),
_r(r),
_size(size),
_animSpeed(animsPeed),
_handleSize(handlesize)
{
	_handle = new int[_size];
	for (int i = 0; i < _size; i++) {
		_handle[i] = handle[i];
	}
	_animCount = 0;
	_currentTime = GetNowCount();
	_animCount = 0;
};

Gate::~Gate() {
	delete[] _handle; _handle = nullptr;
};

void Gate::Process(){
	if (GetNowCount() - _currentTime >= _animSpeed) {
		_currentTime = GetNowCount();
		_animCount++;
		if (_animCount >= _size) {
			_animCount = 0;
		}
	}
};

void Gate::Draw() {
	DrawBillboard3D(_pos, 0.5f, 0.5f, _handleSize, 0.0f, _handle[_animCount], true);
};