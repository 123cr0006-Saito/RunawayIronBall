#pragma once
#include "DxLib.h"
#include "ObjectBase.h"


class CharacterBase : public ObjectBase
{
public: 
	CharacterBase();
	virtual ~CharacterBase();

	virtual bool Init(int modelHandle, VECTOR pos);
	virtual bool Process();
	virtual bool Render();

public:
	int GetModelHandle() { return _modelHandle; };

	virtual void SetPos(VECTOR pos) { this->_pos = pos; };
	VECTOR GetPosition() { return _pos; };
	VECTOR* GetPositionPtr() { return &_pos; };
	VECTOR GetForwardDir() { return _forwardDir; };
	VECTOR*  GetForwardDirPtr() { return &_forwardDir; };
protected:
	int _modelHandle;
	VECTOR _pos;
	VECTOR _forwardDir;
};


