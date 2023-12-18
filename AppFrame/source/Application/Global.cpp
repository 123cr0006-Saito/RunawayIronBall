#include "Global.h"

Global global;

Global::Global() {
resource = new ResourceServer();
effect = new Effect();
exit_count = false;
};

Global::~Global() {

};

bool Global::Init() {
	return true;
};