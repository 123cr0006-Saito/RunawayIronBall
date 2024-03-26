#include "BoardArrow.h"

const unsigned short ::BoardArrow::vertexList[6] = { 0,1,2,2,1,3 };

BoardArrow::BoardArrow(std::string name,float length):
	_handle(ResourceServer::LoadGraph("Arrow", name.c_str()))
{
	_length = length;
	//uvƒŠƒXƒg
	float uvList[4][2] = {
		{0.0 ,0.0 },
		{1.0f,0.0 },
		{0.0 ,1.0f},
		{1.0f,1.0f}
	};
	float Width = 50.0f;
	float posListX[4] = { -Width,Width, -Width ,Width };
	float posListZ[4] = { _length,_length,0.0f,0.0f };
	for (int i = 0; i < 4; i++) {
		_originPos[i] = VGet(posListX[i], 50, posListZ[i]);
		vertex[i].norm = VGet(0,1,0);
		vertex[i].dif = GetColorU8(255, 255, 255, 255);
		vertex[i].spc = GetColorU8(0, 0, 0, 0);
		vertex[i].u = uvList[i][0];
		vertex[i].v = uvList[i][1];
		vertex[i].su = 0.0f;
		vertex[i].sv = 0.0f;
	}
};

BoardArrow::~BoardArrow() {

};

bool BoardArrow::Update(VECTOR pos, float dirY) {
	MATRIX matrix = MGetRotY(dirY);
	for (int i = 0; i < 4; i++) {
		vertex[i].pos = VAdd(pos,VTransform(_originPos[i], matrix));
	}
	return true;
};

bool BoardArrow::Update(VECTOR pos, VECTOR dirVec) {
	float dirY = atan2(dirVec.x, dirVec.z);
	MATRIX matrix = MGetRotY(dirY);
	for (int i = 0; i < 4; i++) {
		vertex[i].pos = VAdd(pos, VTransform(_originPos[i], matrix));
	}
	return true;
};

bool BoardArrow::Render() {
	DrawPolygonIndexed3D(vertex, VERTEX_MAX, vertexList,  2, _handle, true);
	return true;
};