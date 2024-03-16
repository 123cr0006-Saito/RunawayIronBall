#include "ModeTutorial.h"

ModeTutorial::ModeTutorial(int* handle, int size){
	_pageSize = size;
	_handle = new int[_pageSize];
	for(int i = 0; i < _pageSize; i++){
		_handle[i] = handle[i];
	}
	_currentTime = 0;
	_selectItem = 0;
	_input = XInput::GetInstance();
};

bool ModeTutorial::Initialize(){
	return true;
};

bool ModeTutorial::Terminate(){
	delete[] _handle;
	_input = nullptr;
	return true;
};

bool ModeTutorial::Process(){

	if(_input->GetTrg(XINPUT_BUTTON_STICK_LEFT) || _input->GetTrg(XINPUT_BUTTON_DPAD_LEFT)){
	   if(_selectItem < _pageSize){
	      _selectItem++;
	   }
	}

	if (_input->GetTrg(XINPUT_BUTTON_STICK_RIGHT) || _input->GetTrg(XINPUT_BUTTON_DPAD_RIGHT)) {
		if (_selectItem < _pageSize) {
			_selectItem++;
		}
	}

	if(_input->GetTrg(XINPUT_BUTTON_A) && _selectItem >= _pageSize){
		ModeServer::GetInstance()->Del(this);
	}

	return true;
};

bool ModeTutorial::Render(){
	int x,y;
	GetGraphSize(_handle[_selectItem], &x, &y);
	int ScreenX, ScreenY,ScreenDepth;
	GetScreenState(&ScreenX, &ScreenY,&ScreenDepth);
	DrawGraph(ScreenX -x /2, ScreenY - y / 2, _handle[_selectItem], true);
	return true;
};
