#include "UIHeart.h"

const int UIHeart::_damageSeconds = 3000;
const int UIHeart::_maxHp = 4;

UIHeart::UIHeart(VECTOR pos, std::string handleName) : UIBase::UIBase(pos, handleName)
{
	_hp = 4;
	float _x, _y;
	GetGraphSizeF(_handle[_handleNum], &_x, &_y);
	_heart = new DrawGauge(_handle[0], 3, _x, true);
	_currentTime = 0;
	_oldHp = 4;
	_IsDamage = false;
};

UIHeart::UIHeart(VECTOR pos, std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf)
	: UIBase::UIBase(pos, handleName, AllNum, XNum, YNum, XSize, YSize, HandleBuf)
{
	_hp = 4;
	float _x, _y;
	GetGraphSizeF(_handle[_handleNum], &_x, &_y);
	_heart = new DrawGauge(_handle[0], 3, _x, true);
	_currentTime = 0;
	_oldHp = 4;
	_IsDamage = false;
};

UIHeart::UIHeart(VECTOR pos, int size, int* handle, int damageHandleNum) : UIBase::UIBase(pos, size, handle)
{
	_hp = 4;
	float _x, _y;
	GetGraphSizeF(_handle[_handleNum], &_x, &_y);
	_heart = new DrawGauge(_handle[damageHandleNum], 3, _x, true);
	_currentTime = 0;
	_oldHp = 4;
	_IsDamage = false;
}


UIHeart::~UIHeart() {
	//���ɂȂ�
};

void UIHeart::SetDamage() {
	if (_oldHp != _hp) {
		_IsDamage = true;
		_currentTime = GetNowCount();
	}
	_oldHp = _hp;
};

bool  UIHeart::Process() {

	if (XInput::GetInstance()->GetTrg(XINPUT_BUTTON_X)) {
		DecreaseHP();
	}

	SetDamage();

	if (_IsDamage) {
		float _x, _y;
		GetGraphSizeF(_handle[_handleNum], &_x, &_y);
		int nowTime = GetNowCount() - _currentTime;
		VECTOR gaugePos = VAdd(_pos, VGet(_x * _hp + _x / 2, _y / 2, 0));
		//_heart->Process(gaugePos, _damageSeconds - nowTime, _damageSeconds);
		_heart->Process(gaugePos, _damageSeconds, _damageSeconds);
		if (_damageSeconds < nowTime) {
			_IsDamage = false;
		}
	}
	return true;
};

bool  UIHeart::Draw() {
	int _x, _y;
	//�̗͂̕\��
	GetGraphSize(_handle[_handleNum], &_x, &_y);
	for (int i = 0; i < _maxHp; i++) {
		i >= _hp ? _handleNum = 1 : _handleNum = 0;
		if (i != _hp || !_IsDamage) {
			DrawGraph(_pos.x + _x * i, _pos.y, _handle[_handleNum], true);
		}
	}
	//�����Ă���̗͂̕\��
	if (_IsDamage) {
		int nowTime = GetNowCount() - _currentTime;
		if (nowTime % 500 < 250) {
			_heart->Draw();
		}
	};

	return true;
};
