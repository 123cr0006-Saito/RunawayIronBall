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
	//“Á‚É‚È‚µ
};

void UIHeart::SetDamage() {
	if (_oldHp != _hp) {
		_IsDamage = true;
		_currentTime = GetNowCount();
	}
	_oldHp = _hp;
};

bool  UIHeart::Process() {
	SetDamage();

	if(XInput::GetInstance()->GetTrg(XINPUT_BUTTON_X) ){
		DecreaseHP();
	}

	if (_IsDamage) {
		float _x, _y;
		GetGraphSizeF(_handle[_handleNum], &_x, &_y);
		VECTOR gaugePos = VAdd(_pos, VGet(_x * _hp + _x / 2, _y / 2, 0));
		_heart->Process(gaugePos, _damageSeconds - (GetNowCount() - _currentTime), _damageSeconds);
	}
	return true;
};

bool  UIHeart::Draw() {
	int _x, _y;
	//‘Ì—Í‚Ì•\Ž¦
	GetGraphSize(_handle[_handleNum], &_x, &_y);
	for (int i = 0; i < _maxHp; i++) {
		i >= _hp ? _handleNum = 1 : _handleNum = 0;
		DrawGraph(_pos.x + _x * i, _pos.y, _handle[_handleNum], true);
	}
	//Œ¸‚Á‚Ä‚¢‚é‘Ì—Í‚Ì•\Ž¦
	if (_IsDamage) {
		_heart->Draw();
	};

	return true;
};
