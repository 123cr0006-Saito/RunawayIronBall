#pragma once
#include "appframe.h"
class Heart
{
public :
	Heart(VECTOR pos);
	~Heart();
	bool Process();
	bool Render();

	Sphere GetSphere() { return _sphere; }
protected:
	int _model;
	int _currentTime;
	VECTOR _pos;
	Sphere _sphere;
};

