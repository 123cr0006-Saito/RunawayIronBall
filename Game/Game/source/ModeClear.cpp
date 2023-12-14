#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeClear.h"
#include "ModeTitle.h"
bool ModeClear::Initialize(){
	if (!base::Initialize()) { return false; }

	return true;
};

bool ModeClear::Terminate(){
	base::Terminate();
	return true;
};

bool ModeClear::Process(){
	base::Process();
	

	return true;
};

bool ModeClear::Render() {
	base::Render();
	
	return true;
};