
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeTitle.h"
#include "ModeGame.h"
#include "math.h"

bool ModeTitle::Initialize() {
	if (!base::Initialize()) { return false; }

	input = base::GetInputAdd();

	return true;
}

bool ModeTitle::Terminate() {
	// _cg を解放しないといけないけど、AMLibは個別解放できない・・
	base::Terminate();
	return true;
}

bool ModeTitle::Process() {
	base::Process();



	return true;
}

bool ModeTitle::Render() {
	base::Render();

	return true;
}

