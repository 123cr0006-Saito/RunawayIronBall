#include "CrystarPattern4.h"
CrystarPattern4::CrystarPattern4() :EnemyBase::EnemyBase() {

};

CrystarPattern4::~CrystarPattern4() {
	EnemyBase::~EnemyBase();
};

void CrystarPattern4::InheritanceInit() {

	_attackPos = VGet(0, 0, 0);
	_attackDir = 0.0f;
	_nowAttackDistance = 0;
	_attackDistanceSpeed = 3;
};

bool CrystarPattern4::ModeAttack() {
	int nowTime = GetNowCount() - _currentTime;//今の状態になってから何秒経ったか？

	//プランナーさん側で変更できる場所　※秒数単位 
	int rottime = 1.0f * 1000.0f;//攻撃までの回転時間
	int oneLoundTime = 0.7f * 1000.0f;//左右の揺れに対する一周する時間
	int attackRange = 750;//攻撃範囲の半径 cm

	_attackDir -= Math::DegToRad(30.0f);//毎フレーム30度増やす

	if (nowTime >= rottime) {
		//1周する前に半径の伸びが - になるとアルキメデスの法則から外れるので螺旋ではなく正面に飛び出た形になる
		_nowAttackDistance += _attackDistanceSpeed;
		if (_nowAttackDistance <= 0 || attackRange <= _nowAttackDistance) {
			_attackDistanceSpeed = -_attackDistanceSpeed;
			if (_nowAttackDistance <= 0) {
				_modeState = ENEMYTYPE::COOLTIME;
				_currentTime = GetNowCount();
				_nowAttackDistance = 0;
			}
		}
	}
	_attackPos.z = _nowAttackDistance * sin(DX_PI_F * 2.0f * ((float)nowTime / oneLoundTime));
	_attackPos.x = _nowAttackDistance * cos(DX_PI_F * 2.0f * ((float)nowTime / oneLoundTime));

	return true;
};

bool CrystarPattern4::ModeCoolTime() {
	//プランナーさん側で変更できる場所　※秒数単位 
	float moveCoolTime = 2.0f * 1000; //攻撃してからのクールタイム   
	_attackDir += Math::DegToRad(15.0f);//毎フレーム30度増やす
	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_attackDir = 0.0f;
		_currentTime = GetNowCount();
		_modeState = ENEMYTYPE::DISCOVER;
	}
	return true;
};

bool CrystarPattern4::SetState() {
	//最終的なモデルの位置や角度を調整
	if (_model != 0) {
		MV1SetRotationXYZ(_model, VGet(_rotation.x, _rotation.y + _attackDir, 0.0f));
		MV1SetPosition(_model, VAdd(_pos, _attackPos));
	}
	return true;
};

bool CrystarPattern4::DebugRender() {
	DrawSphere3D(VAdd(VAdd(_pos, _diffeToCenter), _attackPos), _r, 16, GetColor(0, 0, 255), GetColor(0, 0, 255), false);
	return true;
};