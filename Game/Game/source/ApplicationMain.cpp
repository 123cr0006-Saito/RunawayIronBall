
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeTest.h"
#include "ModeTitle.h"

// 実体
ApplicationMain				g_oApplicationMain;

bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }

	// モードの登録
	ModeServer::GetInstance()->Add(new ModeTitle(), 1, "Title");
	//ModeServer::GetInstance()->Add(new ModeTest(), 1, "Game");

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

