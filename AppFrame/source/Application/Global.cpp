#include "Global.h"

Global global;

Global::Global() {
resource = new ResourceServer();
_timer = new Timer();
_timer->Start();
exit_count = false;
time = 0;
};

Global::~Global() {
};

bool Global::Init() {
	return true;
};