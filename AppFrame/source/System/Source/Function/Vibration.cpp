#include "../../Header/Function/Vibration.h"
#include "../../Header/Function/mymath.h"
#include "DxLib.h"

Vibration::Vibration(int padID)
{
	int checkID = -1;
	if (DX_INPUT_PAD1 <= padID && padID <= DX_INPUT_PAD4) checkID = padID;
	_padID = checkID;
}

Vibration::~Vibration()
{
}

void Vibration::SetVibrationController(float frame, int power)
{
	float remainingFrame = (float)frame * 1000.0f / 60.0f; // ƒtƒŒ[ƒ€‚ðƒ~ƒŠ•b‚É•ÏŠ· 60fps
	StartJoypadVibration(_padID, power, remainingFrame, -1);
}