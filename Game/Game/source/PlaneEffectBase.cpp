#include "PlaneEffectBase.h"
namespace PlaneEffect {

	const unsigned short PlaneEffect::PlaneEffectBase::vertexOrder[6] = { 0,1,2,2,1,3 };

	PlaneEffectBase::PlaneEffectBase(VECTOR pos, int sizeX, int* handle, int handleMax, int speed) {
		_animCount = 0;
		_animMax = handleMax;
		_handle = new int[_animMax];
		std::swap(_handle, handle);
		_speed = speed;
		_currentTime = GetNowCount();
		_IsPlay = true;
		//�ݒ肵���T�C�Y�Ƃ��Ƃ��Ƃ̉摜�T�C�Y����䗦�ŃT�C�Y������o��
		int x, y;
		GetGraphSize(_handle[0], &x, &y);
		_sizeX = sizeX;
		_sizeY = y * _sizeX / x;
	};

	PlaneEffectBase::~PlaneEffectBase() {
		 _handle = nullptr;
	};

	bool PlaneEffectBase::Update(){
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

}