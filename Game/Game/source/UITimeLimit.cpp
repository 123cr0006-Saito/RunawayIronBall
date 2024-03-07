#include "UITimeLimit.h"
UITimeLimit::UITimeLimit(VECTOR pos,VECTOR numPos, int size, int* numhandle, int colonHandle,int frameHandle) : UIBase(pos, size, numhandle){
	_timeLimit = TimeLimit::GetInstance();
	_colonHandle = colonHandle;
	_frameHandle = frameHandle;
	_numPos = numPos;
};

UITimeLimit::~UITimeLimit(){
	// “Á‚É‚È‚µ
};

bool UITimeLimit::Process(){
	return true;
};

bool UITimeLimit::Draw() {
	DrawGraph(_pos.x, _pos.y, _frameHandle, true);
	int time = _timeLimit->GetTimeLimit();
	int loopCount = 0;

	int x = static_cast<int>(_numPos.x);
	while (1) {

		int handleX, handleY;
		if (loopCount == 2) {
			// ƒRƒƒ“‚ğ•`‰æ
			GetGraphSize(_colonHandle, &handleX, &handleY);
			DrawGraph(x+ handleX, _numPos.y + 10, _colonHandle, true);
			x -= handleX+20;// ”š‚ÌŠÔŠu
		}
		
		// •b”‚ğ•`‰æ
		int num = time % 10;
		DrawGraph(x, _numPos.y, _handle[num], true);
		time /= 10;

		GetGraphSize(_handle[num], &handleX, &handleY);

		x -= handleX+10;// ”š‚ÌŠÔŠu
		loopCount++;// ‰½Œ…–Ú‚©”‚¦‚é

		if (time == 0) {
			break;
		}

	}
	return true;
};