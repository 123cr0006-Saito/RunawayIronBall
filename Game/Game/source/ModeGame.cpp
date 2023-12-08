
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeGameOver.h"

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }



	return true;
}

bool ModeGame::Terminate() {
	base::Terminate();
	return true;
}

bool ModeGame::Process() {
	base::Process();
	

	return true;
}

bool ModeGame::Render() {
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	// ÉâÉCÉgê›íË
	SetUseLighting(TRUE);
	clsDx();
	

	SetUseZBuffer3D(FALSE);
	return true;
}

