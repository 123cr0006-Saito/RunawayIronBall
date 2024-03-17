#include "PlaneEffectBase.h"


	const unsigned short PlaneEffectBase::vertexOrder[6] = { 0,1,2,2,1,3 };

	PlaneEffectBase::PlaneEffectBase(VECTOR pos, int sizeX, int* handle, int handleMax, int speed) {
		_animCount = 0;
		_animMax = handleMax;
		_handle = new int[_animMax];
		for (int i = 0; i < _animMax; i++) {
			_handle[i] = handle[i];
		}
		_speed = speed;
		_currentTime = GetNowCount();
		//�ݒ肵���T�C�Y�Ƃ��Ƃ��Ƃ̉摜�T�C�Y����䗦�ŃT�C�Y������o��
		int x, y;
		GetGraphSize(_handle[0], &x, &y);
		_sizeX = sizeX;
		_sizeY = y * _sizeX / x;
	};

	PlaneEffectBase::~PlaneEffectBase() {
		delete[] _handle;  _handle = nullptr;
	};

	bool PlaneEffectBase::Process(){
		if (GetNowCount() - _currentTime >= _speed) {
			_currentTime = GetNowCount();
			_animCount++;
			if (_animCount > _animMax) {
				_IsPlay = false;
			}
		}
		return true;
	};

	bool PlaneEffectBase::Render() {
		return true;
	};

