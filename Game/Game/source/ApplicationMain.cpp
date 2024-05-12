#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeClear.h"
#include "ModeTitle.h"
#include "ModeGameOver.h"
#include "ModeScenario.h"
#include "ModeBossBattle.h"

// ����
ApplicationMain				g_oApplicationMain;

bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }
	// �����̓ǂݍ���
	global.SoundLoad();
	// ���[�h�̓o�^
	ModeServer::GetInstance()->Add(NEW ModeTitle(), 1, "Title");
	// �R���g���[���[�̏�����
	_input = NEW XInput(DX_INPUT_PAD1);
	// FPS�����肳���邽�߂̃N���X��������
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
	return true;
}

