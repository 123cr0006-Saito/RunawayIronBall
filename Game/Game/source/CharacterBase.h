#pragma once
#include "DxLib.h"


class CharacterBase
{
public: // 
	CharacterBase(int modelHandle, VECTOR pos);
	virtual ~CharacterBase();

	virtual bool Process();
	virtual bool Render();

public:
	int GetModelHandle() { return _modelHandle; };

	void SetPos(VECTOR pos) { this->_pos = pos; };
	VECTOR GetPosition() { return _pos; };
	VECTOR* GetPositionPtr() { return &_pos; };
	VECTOR*  GetForwardDir() { return &_forwardDir; };
protected:
	int _modelHandle;
	VECTOR _pos;
	VECTOR _forwardDir;
};


