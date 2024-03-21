#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeClear.h"
#include "ModeTest.h"
#include "ModeTitle.h"
#include "ModeGameOver.h"
#include "ModeScenario.h"
#include "ModeBossBattle.h"

// ŽÀ‘Ì
ApplicationMain				g_oApplicationMain;

bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }

	// ƒ‚[ƒh‚Ì“o˜^
	ModeServer::GetInstance()->Add(NEW ModeTitle(), 1, "Title");
	//ModeServer::GetInstance()->Add(NEW ModeGameOver(), 1, "Title");
	//global.ResourceLoad();
	//ModeServer::GetInstance()->Add(NEW ModeClear(), 1, "Clear");
	//ModeServer::GetInstance()->Add(NEW ModeScenario("Data/ScenarioData/Scenario01.csv",1), 2, "Scenario");
	//ModeServer::GetInstance()->Add(NEW ModeGame(), 1, "Game");
	//ModeServer::GetInstance()->Add(NEW ModeBossBattle(), 1, "BossBattle");

	global.Init();

	_input = NEW XInput(DX_INPUT_PAD1);
	_fpsController = NEW Fps();
	return true;
}

bool ApplicationMain::Terminate() {
	base::Terminate();
	delete _input;
	delete _fpsController;
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
	//_fpsController->DrawFps(0, 0);
	return true;
}

