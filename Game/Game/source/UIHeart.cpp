#include "UIHeart.h"

UIHeart::UIHeart(VECTOR pos, std::string handleName) : UIBase::UIBase(pos, handleName)
{
	_hp = 4;
	//特になし
};

UIHeart::UIHeart(VECTOR pos, std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf)
	: UIBase::UIBase(pos, handleName, AllNum, XNum, YNum, XSize, YSize, HandleBuf)
{
	_hp = 4;
	//特になし
};


UIHeart::~UIHeart() {
	//特になし
};

bool  UIHeart::Process() {
	//特になし
	//アニメーションとか拡大サイズとか
	return true;
};

bool  UIHeart::Draw() {
	int _x, _y;
	GetGraphSize(_handle[_handleNum], &_x, &_y);
	for (int i = 0; i < _hp; i++) {
		DrawGraph(_pos.x + _x * i, _pos.y, _handle[_handleNum], true);
		if (i ==_hp - 1) {
			MyDrawModiGraph(MGetIdent(), VAdd(_pos, VGet(_cx + _x * i, _cy, 0)), 1.0f, 0, _x, _y, _handle[_handleNum]);
		}
	}
	return true;
};