#include "EnemyBase.h"
EnemyBase::EnemyBase() {
	_model = 0;
	_pos = VGet(0, 0, 0);
	_speed = 0;
	_sartchRange = 0.0f;
	_state = TYPE::search;
	_direction = 0.0f;
	_flontAngle = Math::DegToRad(30);//‚Æ‚è‚ ‚¦‚¸‘SƒLƒƒƒ‰“ˆê30“x
	_moveRange = 0.0f;
	 _orignPos = _pos;
	 _nextMovePoint = _pos;
	 _r = 0.0f;
};

EnemyBase::~EnemyBase() {
	MV1DeleteModel(_model);
};

bool EnemyBase::StopPos() {
	if (_pos.x >= _nextMovePoint.x - _speed && _pos.x <= _nextMovePoint.x + _speed &&
		_pos.y >= _nextMovePoint.y - _speed && _pos.y <= _nextMovePoint.y + _speed &&
		_pos.z >= _nextMovePoint.z - _speed && _pos.z <= _nextMovePoint.z + _speed)
	{
		return true;
	}
	return false;
};

void EnemyBase::CollisionDetection() {
	/*if(_state != TYPE::discover) {
		VECTOR v_length = VSub(_player->GetPos(), _pos);
		MATRIX matrix = Math::MMultXYZ(0.0f, _direction, 0.0f);
		VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
		VECTOR pla_dir = VNorm(v_length);
		float range_dir = Math::CalcVectorAngle(ene_dir, pla_dir);
		float len = VSize(v_length);
		if(VSize(v_length) <= _sartchRange) {
			if(range_dir <= _flontAngle) {
				_state = TYPE::discover;
			}
		}
	}*/
}

bool EnemyBase::Process(){
	return true;
};

bool EnemyBase::Render() {
	return true;
};