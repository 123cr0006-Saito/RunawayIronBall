#pragma once
#include "appframe.h"
#include <string>
class BoardArrow
{
public :
	BoardArrow(std::string name);
	~BoardArrow();
	bool Update(VECTOR pos, float dirY, float length, float katamuki);
	bool Render();
protected:
	int _handle;
	static const int ONE_ROW_POLYGON_MAX = 100;
	static const int VERTEX_MAX = 4;
	unsigned short vertexOrder[ONE_ROW_POLYGON_MAX * 6];
	VERTEX3D vertex[ONE_ROW_POLYGON_MAX / 2 * VERTEX_MAX + 2];
};

