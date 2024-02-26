#include "UISuppressionGauge.h"

const unsigned short UISuppressionGauge::vertex[6]{ 0,1,2,2,1,3 };

UISuppressionGauge::UISuppressionGauge(VECTOR pos, int size, int* handle) : UIBase::UIBase( pos,  size,  handle){
	//     x,      y,    u,    v
	float posTbl[][4] = {
		{-_cx,-_cy,0.0f,0.0f},
		{_cx,-_cy,1.0f,0.0f},
		{-_cx,_cy,0.0f,1.0f},
		{_cx,_cy,1.0f,1.0f}
	};

	VECTOR center = VAdd(pos, VGet(_cx, _cy, 0));

	for (int i = 0; i < 4; i++) {
		_suppression[i].pos = VAdd(center, VGet(posTbl[i][0], posTbl[i][1], 0));
		_suppression[i].u = posTbl[i][2];
		_suppression[i].v = posTbl[i][3];
		_suppression[i].dif = GetColorU8(255, 255, 255, 255);
		_suppression[i].rhw = 1.0f;

		_residue[i].pos = VAdd(center, VGet(posTbl[i][0], posTbl[i][1], 0));
		_residue[i].u = posTbl[i][2];
		_residue[i].v = posTbl[i][3];
		_residue[i].dif = GetColorU8(125, 125, 125, 255);
		_residue[i].rhw = 1.0f;
	}

	GetGraphSizeF(handle[2], &_ironHandleSize.first.x, &_ironHandleSize.first.y);
	_ironHandleSize.second = _ironHandleSize.first.y / 2;
};

UISuppressionGauge::~UISuppressionGauge() {
		//“Á‚É‚È‚µ
};

bool UISuppressionGauge::Process(){
	// ^‚ñ’†‚Ì’¸“_2‚Â‚ÌÀ•W‚Æ‚•‚ð•Ï‚¦‚½‚çŽÀ‘•‚Å‚«‚é
	for (int i = 0; i < 2; i++) {
		_suppression[0 + 2 * i].pos = VGet(0, 0, 0);
		_suppression[0 + 2 * i].u = 1.0f;
		_residue[1 + 2 * i].pos = VGet(0,0,0);
		_residue[1 + 2 * i].u = 1.0f;
	}
	return true;
};

bool UISuppressionGauge::Draw() {
	VERTEX2D* list[2] = { _suppression,_residue};
	for (int i = 0; i < 2; i++) {
		DrawPrimitiveIndexed2D(list[i], 4, vertex, 6, DX_PRIMTYPE_TRIANGLELIST, _handle[i], true);
	}
	DrawGraph(_ironHandleSize.first.x, _ironHandleSize.first.y + _ironHandleSize.second, _handle[2], true);
	return true;
};