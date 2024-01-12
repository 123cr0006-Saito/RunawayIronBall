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
	//特になし
};

bool UIExpPoint::Process() {

	//--------------------------------------------------------------
	//aとbはデバッグ用です　後で消します　a:今の経験値 100:次の経験値 ｂ:線形補間
	if (CheckHitKey(KEY_INPUT_DOWN)) {
		a--;
	}
	else if (CheckHitKey(KEY_INPUT_UP)) {
		a++;
	}
	//----------------------------------------------------------------

	//※ このやり方は使用する画像のｙに隙間がないことが前提で組まれています。
	//経験値の引き継ぎ方はメインプログラマーと要相談

	//a:今の値 b:次のレベルまでの経験値
	//経験値をオーバーした時のためのクランプ
	//今は100次のレベルまでの経験値を１００に設定
	a = Math::Clamp(0, 100, a);
	float b = a / 100.0f;

	for (int i = 0; i < 2; i++) {
		//上にある座標を一度下におろしてから計算する
		_front[i].pos.y = _pos.y + _cy * 2 - _cy * b * 2;
		_front[i].v = 1.0f - b;
	}

	return true;
};

bool UIExpPoint::Draw() {
	//背景の表示
	//背景は色を暗くするため頂点指定の方で描画
	//drawgraphで暗くできるならそっちのほうが良い
	DrawPrimitiveIndexed2D(_back, 4, vertex, 6, DX_PRIMTYPE_TRIANGLELIST, _handle[_handleNum], true);
	//経験値の表示
	DrawPrimitiveIndexed2D(_front, 4, vertex, 6, DX_PRIMTYPE_TRIANGLELIST, _handle[_handleNum], true);
	//デバッグ用 uv座標のvがどこにあるかの確認用
	for (int i = 0; i < 4; i++) {
		DrawPixel(_front[i].pos.x, _front[i].pos.y, GetColor(rand() % 255, rand() % 255, rand() % 255));
	}
	return true;
};