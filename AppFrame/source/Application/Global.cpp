#include "Global.h"

Global global;

Global::Global() {
_timer = new Timer();
_timer->Start();
exit_count = false;
};

Global::~Global() {
	delete _timer;
};

bool Global::Init() {
	return true;
};