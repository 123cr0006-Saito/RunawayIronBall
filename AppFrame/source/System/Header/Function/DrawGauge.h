#pragma once
#include"dxlib.h"
#include "mymath.h"
#include "../../../Application/Global.h"
#include "math.h"
class DrawGauge
{
public:
	DrawGauge(int image, int start_pos, float size, bool flag);
	DrawGauge(const char* name, int start_pos, float size, bool flag);
	~DrawGauge();
	bool Process(VECTOR pos, float parcent, float parcent_max);//今の値と最大の値
	bool Process(VECTOR pos, float parcent, float parcent_max,float size);//サイス指定版
	bool Draw();
	bool Draw(int handle);

	static  const unsigned short _rightTraianglList[];//右回転から始めえるための描画の時に使う各頂点のリスト
	static  const unsigned short _leftTraianglList[];//左回転から始めえるための描画の時に使う各頂点のリスト
private:
	VERTEX2D vertex[10];//描画の際に使う各頂点
	int handle;//ゲージとして使う画像　読み込みの場合はリソースサーバーを使用
	int _startPosition;//始まる位置　始まる位置はＴＹＰＥ
	int _transFlag;//反転させるか
	float _size;//表示する画像の半径

	//開始地点を列挙
	enum TYPE : int {
		right = 0,
		down,
		left,
		up
	};
};

