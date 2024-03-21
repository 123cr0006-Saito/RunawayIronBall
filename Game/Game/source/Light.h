#pragma once
#include "appframe.h"
class Light
{
public :
	Light(std::string fileName);
	~Light();
	bool LoadLightColor(std::string fileName);
protected:
	int _lightHandle;
};

