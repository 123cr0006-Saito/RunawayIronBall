#include "Suppression.h"

Suppression* Suppression::_instance = nullptr;

Suppression::Suppression(){
	_instance = this;
	_maxSuppression = 0;
	_nowSuppression = 0;
};

Suppression::~Suppression() {
	_instance = nullptr;
};

void Suppression::AddSuppression(int suppression) { 
	_maxSuppression += suppression;
	_nowSuppression += suppression;
}

void Suppression::SubSuppression(int suppression) { if(_nowSuppression > 0)_nowSuppression -= suppression; }

void Suppression::ClearSuppression() {
	_maxSuppression = 0;
	_nowSuppression = 0;
};

bool Suppression::GetIsRatio() {
	// max‚Ì20%ˆÈ‰º‚È‚çtrue
	if (_nowSuppression <= static_cast<float>(_maxSuppression / 10 * (10 - 8))) {
		return true;
	}
	return false;
}