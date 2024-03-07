#pragma once
#include "Cell.h"
class Cell;

class ObjectBase
{
public:
	ObjectBase();
	virtual ~ObjectBase();

	Cell* _cell;
};