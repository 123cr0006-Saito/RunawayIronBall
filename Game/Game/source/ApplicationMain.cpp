
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeTitle.h"

// 実体
ApplicationMain				g_oApplicationMain;

bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }

	// モードの登録
	ModeServer::GetInstance()->Add(new ModeTitle(), 1, "Title");
	global.Init();
	return true;
}

bool ApplicationMain::Terminate() {
	base::Terminate();
	return true;
}

bool ApplicationMain::Input() {
	base::Input();
	return true;
}

bool ApplicationMain::Process() {
	base::Process();
	return true;
}

bool ApplicationMain::Render() {
	base::Render();
	return true;
}

