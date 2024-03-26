#pragma once
#include "appframe.h"
#include <string>
class BoardArrow
{
public:
	BoardArrow(std::string name, float length);
	~BoardArrow();
	bool Update(VECTOR pos, float dirY); // ｙ軸角度
	bool Update(VECTOR pos, VECTOR dirVec); // 方向ベクトル
	bool Render();
protected:
	int _handle;
	float _length;
	static const int VERTEX_MAX = 4;
	static const unsigned short vertexList[6];
	VERTEX3D vertex[VERTEX_MAX];
	VECTOR _originPos[4];
};

