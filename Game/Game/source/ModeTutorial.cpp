#include "ModeTutorial.h"
#include "TimeLimit.h"
ModeTutorial::ModeTutorial(int* handle, int size){
	_pageSize = size;
	_handle = new int[_pageSize];
	for(int i = 0; i < _pageSize; i++){
		_handle[i] = handle[i];
	}
	_selectItem = 0;
	_input = XInput::GetInstance();
};

bool ModeTutorial::Initialize(){
	TimeLimit::GetInstance()->Stop();
	return true;
};

bool ModeTutorial::Terminate(){
	delete[] _handle;
	_input = nullptr;
	TimeLimit::GetInstance()->Restart();
	return true;
};

bool ModeTutorial::Process(){
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	ModeServer::GetInstance()->PauseProcessUnderLayer();
	if (_input->GetTrg(XINPUT_BUTTON_A) ) {
		if (_selectItem < _pageSize) {
			_selectItem++;
			if (_selectItem == _pageSize) {
				ModeServer::GetInstance()->Del(this);
			}
		}
	}

	return true;
};

bool ModeTutorial::Render(){
	if (!ModeServer::GetInstance()->IsAboutLayer(this)) {
		int x, y;
		GetGraphSize(_handle[_selectItem], &x, &y);
		int ScreenX, ScreenY, ScreenDepth;
		GetScreenState(&ScreenX, &ScreenY, &ScreenDepth);
		DrawGraph((ScreenX - x) / 2, (ScreenY - y) / 2, _handle[_selectItem], true);
	}
	return true;
};
