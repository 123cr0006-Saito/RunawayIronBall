#pragma once

class Vibration
{
public:
	Vibration(int padID);
	~Vibration();

	void Process();

	void StartVibration(int frame, int power);
	void StopVibration();

private:
	int _padID;

	bool _isVibrating;
	int _remainingFrame;
	int _power;
};