
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeTitle.h"

// ŽÀ‘Ì
ApplicationMain				g_oApplicationMain;

bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }

	// ƒ‚[ƒh‚Ì“o˜^
	//ModeServer::GetInstance()->Add(new ModeTitle(), 1, "Title");
	ModeServer::GetInstance()->Add(new ModeGame(), 1, "Game");
	global.Init();

	_input = new XInput(DX_INPUT_PAD1);
	_fpsController = new Fps();
	return true;
}

bool ApplicationMain::Terminate() {
	base::Terminate();
	ResourceServer::DeleteResourceAll();
	return true;
}

bool ApplicationMain::Input() {
	base::Input();
	_input->Input();
	return true;
}

bool ApplicationMain::Process() {
	base::Process();
	_fpsController->WaitFps();
	return true;
}

bool ApplicationMain::Render() {
	base::Render();
	_fpsController->DrawFps(0, 0);
	return true;
}

