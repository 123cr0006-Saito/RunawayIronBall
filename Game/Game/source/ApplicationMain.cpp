
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeTest.h"
#include "ModeTitle.h"
#include "ModeGameOver.h"
#include "ModeScenario.h"

// ŽÀ‘Ì
ApplicationMain				g_oApplicationMain;

bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }

	// ƒ‚[ƒh‚Ì“o˜^
	//ModeServer::GetInstance()->Add(new ModeTitle(), 1, "Title");
	//ModeServer::GetInstance()->Add(new ModeGameOver(), 1, "Title");
	//ModeServer::GetInstance()->Add(new ModeTest(), 1, "Game");
	
	ModeServer::GetInstance()->Add(new ModeScenario("Data/ScenarioData/Scenario_01.csv"), 1, "Scenario");
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

