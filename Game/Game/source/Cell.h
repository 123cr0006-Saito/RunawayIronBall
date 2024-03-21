#pragma once
#include "ObjectBase.h"


enum OBJ_TYPE {
	NONE,
	PL,
	PL_IB,
	PL_IB_CHAIN,
	EN,
	EN_IB,
	BLDG,
	TWR,
	TWR_PRT,
};

class ObjectBase;
class Cell
{
public:
	Cell() {
		_segment = nullptr;
		_obj = nullptr;
		_objType = OBJ_TYPE::NONE;
		_prev = nullptr;
		_next = nullptr;
	}
	~Cell() {
		_segment = nullptr;
		_obj = nullptr;
		_prev = nullptr;
		_next = nullptr;
	}

	Cell* _segment;
	ObjectBase* _obj;
	OBJ_TYPE _objType;

	Cell* _prev;
	Cell* _next;
};