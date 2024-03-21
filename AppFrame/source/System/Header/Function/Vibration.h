#pragma once

class Vibration
{
public:
	Vibration(int padID);
	~Vibration();
	void SetVibrationController(float frame, int power);

private:
	int _padID;
};