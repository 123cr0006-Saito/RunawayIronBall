//----------------------------------------------------------------------
// @filename BoardParabola.h
// —date: 2024/02/01
// —author: saito ko
// @explanation
// w’è‚Ì•ûŒü‚É•ú•¨ü‚Ì–îˆó‚ğ•`‰æ‚·‚éƒNƒ‰ƒX
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include <string>
class BoardParabola
{
public:
	BoardParabola(std::string name);
	~BoardParabola();
	bool Update(VECTOR pos, float dirY, float length, float inclination); // •ú•¨ü
	bool Render();
protected:
	int _handle;
	static const int ONE_ROW_POLYGON_MAX = 100;
	static const int VERTEX_MAX = 4;
	unsigned short vertexOrder[ONE_ROW_POLYGON_MAX * 6];
	VERTEX3D vertex[ONE_ROW_POLYGON_MAX / 2 * VERTEX_MAX + 2];
};

