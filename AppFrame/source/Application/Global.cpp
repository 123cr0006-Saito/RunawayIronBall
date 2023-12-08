#include "Global.h"

Global global;

Global::Global() {
resource = new ResourceServer();
exit_count = false;
time = 0;
};

Global::~Global() {

};

bool Global::Init() {
	return true;
};