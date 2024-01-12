#include "UIExpPoint.h"

const unsigned short UIExpPoint::vertex[6]{ 0,1,2,2,1,3 };

UIExpPoint::UIExpPoint(VECTOR pos, std::string handleName) :
	UIBase(pos, handleName)
{
	//     x,      y,    u,    v
	float posTbl[][4] = {
		{-_cx,-_cy,0.0f,0.0f},
		{_cx,-_cy,1.0f,0.0f},
		{-_cx,_cy,0.0f,1.0f},
		{_cx,_cy,1.0f,1.0f}
	};

	VECTOR center = VAdd(pos, VGet(_cx, _cy, 0));

	for (int i = 0; i < 4; i++) {
		_back[i].pos = VAdd(center, VGet(posTbl[i][0], posTbl[i][1], 0));
		_back[i].u = posTbl[i][2];
		_back[i].v = posTbl[i][3];
		_back[i].dif = GetColorU8(125, 125, 125, 255);
		_back[i].rhw = 1.0f;

		_front[i].pos = VAdd(center, VGet(posTbl[i][0], posTbl[i][1], 0));
		_front[i].u = posTbl[i][2];
		_front[i].v = posTbl[i][3];
		_front[i].dif = GetColorU8(255, 255, 255, 255);
		_front[i].rhw = 1.0f;
	}
};

UIExpPoint::UIExpPoint(VECTOR pos, std::string handleName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf) :
	UIBase(pos, handleName, AllNum, XNum, YNum, XSize, YSize, HandleBuf)
{
	//     x,      y,    u,    v
	float posTbl[][4] = {
		{-_cx,-_cy,0.0f,0.0f},
		{_cx,-_cy,1.0f,0.0f},
		{-_cx,_cy,0.0f,1.0f},
		{_cx,_cy,1.0f,1.0f}
	};

	VECTOR center = VAdd(pos, VGet(_cx, _cy, 0));

	for (int i = 0; i < 4; i++) {
		_back[i].pos = VAdd(center, VGet(posTbl[i][0], posTbl[i][1], 0));
		_back[i].u = posTbl[i][2];
		_back[i].v = posTbl[i][3];
		_back[i].dif = GetColorU8(125, 125, 125, 255);
		_back[i].rhw = 1.0f;

		_front[i].pos = VAdd(center, VGet(posTbl[i][0], posTbl[i][1], 0));
		_front[i].u = posTbl[i][2];
		_front[i].v = posTbl[i][3];
		_front[i].dif = GetColorU8(255, 255, 255, 255);
		_front[i].rhw = 1.0f;
	}

}

UIExpPoint::~UIExpPoint() {
	//���ɂȂ�
};

bool UIExpPoint::Process() {

	//--------------------------------------------------------------
	//a��b�̓f�o�b�O�p�ł��@��ŏ����܂��@a:���̌o���l 100:���̌o���l ��:���`���
	if (CheckHitKey(KEY_INPUT_DOWN)) {
		a--;
	}
	else if (CheckHitKey(KEY_INPUT_UP)) {
		a++;
	}
	//----------------------------------------------------------------

	//�� ���̂����͎g�p����摜�̂��Ɍ��Ԃ��Ȃ����Ƃ��O��őg�܂�Ă��܂��B
	//�o���l�̈����p�����̓��C���v���O���}�[�Ɨv���k

	//a:���̒l b:���̃��x���܂ł̌o���l
	//�o���l���I�[�o�[�������̂��߂̃N�����v
	//����100���̃��x���܂ł̌o���l���P�O�O�ɐݒ�
	a = Math::Clamp(0, 100, a);
	float b = a / 100.0f;

	for (int i = 0; i < 2; i++) {
		//��ɂ�����W����x���ɂ��낵�Ă���v�Z����
		_front[i].pos.y = _pos.y + _cy * 2 - _cy * b * 2;
		_front[i].v = 1.0f - b;
	}

	return true;
};

bool UIExpPoint::Draw() {
	//�w�i�̕\��
	//�w�i�͐F���Â����邽�ߒ��_�w��̕��ŕ`��
	//drawgraph�ňÂ��ł���Ȃ炻�����̂ق����ǂ�
	DrawPrimitiveIndexed2D(_back, 4, vertex, 6, DX_PRIMTYPE_TRIANGLELIST, _handle[_handleNum], true);
	//�o���l�̕\��
	DrawPrimitiveIndexed2D(_front, 4, vertex, 6, DX_PRIMTYPE_TRIANGLELIST, _handle[_handleNum], true);
	//�f�o�b�O�p uv���W��v���ǂ��ɂ��邩�̊m�F�p
	for (int i = 0; i < 4; i++) {
		DrawPixel(_front[i].pos.x, _front[i].pos.y, GetColor(rand() % 255, rand() % 255, rand() % 255));
	}
	return true;
};