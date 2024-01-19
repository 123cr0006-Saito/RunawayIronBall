#include "SlideUI.h"

LoadSlideUI::LoadSlideUI(std::string handleName, std::string JsonFileName) {
	myJson json(JsonFileName);
	_value = new Param[json._size];
	int i = 0;
	for (auto&& list : json._json) {
		list.at("x").get_to(_value[i]._nextX);
		list.at("y").get_to(_value[i]._nextY);
		list.at("travelTime").get_to(_value[i]._travelTime);
		list.at("waitingTime").get_to(_value[i]._waitingTime);
		i++;
	}
	_x = 0;
	_y = 0;
	_handle = ResourceServer::LoadGraph(handleName);
	_currentTime = GetNowCount();
	_size = json._size;
	_IsUse = true;
	_moveNum = 0;

	SlideUI::GetInstance()->_UIList.push_back(this);
};

LoadSlideUI::~LoadSlideUI(){
	delete[] _value; _value = nullptr;
};

bool LoadSlideUI::Process() {
	if (_IsUse) {
		int nowTime = GetNowCount() - _currentTime;
		if (nowTime <= _value[_moveNum]._travelTime) {
			_x = Easing::Linear(nowTime, _value[_moveNum]._nextX, _value[_moveNum + 1]._nextX, _value[_moveNum]._travelTime);
			_y = Easing::Linear(nowTime, _value[_moveNum]._nextY, _value[_moveNum + 1]._nextY, _value[_moveNum]._travelTime);
		}
		else {
			if (nowTime - _value[_moveNum]._travelTime >= _value[_moveNum+1]._waitingTime) {
				_currentTime = GetNowCount();
				_moveNum++;
			}
		}

		if (_moveNum >= _size) {
			_IsUse = false;
		}

	}
	return true;
};

bool LoadSlideUI::Draw() {
	DrawGraph(_x, _y, _handle, true);
	return true;
};

//------------------------------------------------------------------------------------------------------------------------------

SlideUI* SlideUI::instance = nullptr;
bool SlideUI::Process() {
	for (auto itr = _UIList.begin(); itr != _UIList.end();) {
		if ((*itr)->GetUse()) {
			(*itr)->Process();
			++itr;
		}
		else {
			itr = _UIList.erase(itr);
		}
	}
	return true;
};

bool SlideUI::Draw() {
	for (auto&& list : _UIList) {
		list->Draw();
	}
	return true;
};
