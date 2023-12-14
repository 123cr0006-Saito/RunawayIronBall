#include "ModeGameOver.h"
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeTitle.h"
bool ModeGameOver::Initialize() {
	if (!base::Initialize()) { return false; }

	return true;
};

bool ModeGameOver::Terminate() {
	base::Terminate();
	return true;
};

bool ModeGameOver::Process() {
	base::Process();

	return true;
};

bool ModeGameOver::Render() {
	base::Render();
	
	return true;
};