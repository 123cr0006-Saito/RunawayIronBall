#include "../../Header/Function/Vibration.h"
#include "../../Header/Function/mymath.h"
#include "DxLib.h"

Vibration::Vibration(int padID)
{
	int checkID = -1;
	if (DX_INPUT_PAD1 <= padID && padID <= DX_INPUT_PAD4) checkID = padID;
	_padID = checkID;

	_isVibrating = false;
	_remainingFrame = 0;
	_power = 0;
}

Vibration::~Vibration()
{
}

void Vibration::Process()
{
	if (_isVibrating) {
		_remainingFrame--;

		if (_remainingFrame < 0) {
			StopVibration();
		}
	}
}

void Vibration::StartVibration(int frame, int power)
{
	_isVibrating = true;
	_remainingFrame = frame;
	_power = power;

	StartJoypadVibration(_padID, power, -1, -1);
}

void Vibration::StopVibration()
{
	StopJoypadVibration(_padID);
}
