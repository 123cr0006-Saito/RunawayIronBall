//----------------------------------------------------------------------
// @filename BoardArrow.h
// date: 2024/02/01
// author: saito ko
// @explanation
// wθΜϋόΙξσπ`ζ·ιNX
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include <string>
class BoardArrow
{
public:
	BoardArrow(std::string name, float length);
	~BoardArrow();
	bool Process(VECTOR pos, float dirY); //²px
	bool Process(VECTOR pos, VECTOR dirVec); // ϋόxNg
	bool Render();
protected:
	int _handle;// ζnh
	float _length;// ξσΜ·³
	static const int VERTEX_MAX = 4;// Έ_
	static const unsigned short vertexList[6];// Έ_Xg
	VERTEX3D vertex[VERTEX_MAX];// Έ_ξρ
	VECTOR _originPos[VERTEX_MAX];// ξσΜ΄_
};