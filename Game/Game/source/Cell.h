#pragma once
#include "ObjectBase.h"
//#include "EnemyBase.h"


enum OBJ_TYPE {
	NONE,
	PL,
	PL_IB,
	EN,
	EN_IB
};

class ObjectBase;
class Cell
{
public:
	Cell() {
		_segment = nullptr;
		//_enObj = nullptr;
		_obj = nullptr;
		_objType = OBJ_TYPE::NONE;
		_prev = nullptr;
		_next = nullptr;
	}
	~Cell() {
		_segment = nullptr;
		//_enObj = nullptr;
		_obj = nullptr;
		_prev = nullptr;
		_next = nullptr;
	}

	Cell* _segment;
	//EnemyBase* _enObj;
	ObjectBase* _obj;
	OBJ_TYPE _objType;

	Cell* _prev;
	Cell* _next;
};