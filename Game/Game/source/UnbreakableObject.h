#pragma once
#include "appframe.h"

class UnbreakableObject
{
public:
	UnbreakableObject();
	virtual ~UnbreakableObject();

	void Init(int modelHandle, VECTOR startPos, VECTOR rotation, VECTOR scale, VECTOR obbLength);
	void Process();
	void Render();

	VECTOR GetPos() { return _pos; }

	// �f�o�b�O���̕\��
	void DrawDebugInfo();

protected:
	int _modelHandle;
	VECTOR _pos;

	OBB _obbCol;
};