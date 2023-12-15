#include "enemy.h"

Enemy::Enemy(int modelHandle, VECTOR pos) : CharacterBase(modelHandle, pos)
{
	_speed = 2.0f;
	_startPos = pos;
	MV1SetScale(_modelHandle, VGet(8.0f, 8.0f, 8.0f));

	_cnt = 0;
	_isHit = false;
}

Enemy::~Enemy()
{
}

bool Enemy::Process(VECTOR playerPos)
{
	VECTOR vDir = VSub(playerPos, _pos);
	if (VSize(vDir) > 0.0f) {
		vDir = VNorm(vDir);
		_pos = VAdd(_pos, VScale(vDir, _speed));
		Math::SetModelForward_RotationY(_modelHandle, vDir);
	}

	if (_isHit) {
		if (_blastOffPower > 0.0f) {
			VECTOR tmp = VNorm(_blastOffDir);
			if (VSize(tmp) > 0.0f) {
				_pos = VAdd(_pos, VScale(tmp, _blastOffPower));
			}
			//_blastOffPower -= 1.0f;
			_cnt++;
			if (_cnt > 180) {
				_isHit = false;
				_pos = _startPos;
			}
		}
	}

	MV1SetPosition(_modelHandle, _pos);

	return true;
}
