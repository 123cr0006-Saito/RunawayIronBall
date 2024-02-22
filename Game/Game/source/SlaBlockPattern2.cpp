#include "SlaBlockPattern2.h"
SlaBlockPattern2::SlaBlockPattern2() :EnemyBase::EnemyBase()
{
	//デバック時登録
	//_model = ResourceServer::MV1LoadModel("res/katatumuri/snail.mv1");
};

SlaBlockPattern2::~SlaBlockPattern2() {
	EnemyBase::~EnemyBase();
};

void SlaBlockPattern2::InheritanceInit() {
	_fallCount = 0;
};

bool SlaBlockPattern2::ModeAttack() {
	int nowTime = GetNowCount() - _currentTime;//今の状態になってから何秒経ったか？

	//プランナーさん側で変更できる場所　※秒数単位 
	int enemyRigidityTime = 1.0f * 1000; //攻撃モーションに入っての硬直時間
	int enemyToPlayerPosFrame = 20.0f / 60.0f * 1000;//敵がプレイヤーの位置につくまでの時間
	int fallTime = 1.0f * 1000;//落下するまでの時間

	// 1秒待ってから20フレームでプレイヤーの頭上に到着
	if (_fallCount == 0) {
		if (enemyRigidityTime <= nowTime && nowTime < enemyRigidityTime + enemyToPlayerPosFrame) {
			_pos.x = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.x, _saveNextPoint.x, enemyToPlayerPosFrame);
			_pos.y = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.y, _saveNextPoint.y, enemyToPlayerPosFrame);
			_pos.z = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.z, _saveNextPoint.z, enemyToPlayerPosFrame);
		}
	}
	else {
		fallTime /= 2;
		enemyRigidityTime /= 2;
		if (enemyRigidityTime <= nowTime && nowTime <= enemyRigidityTime + enemyToPlayerPosFrame) {
			_pos.x = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.x, _saveNextPoint.x, enemyToPlayerPosFrame);
			_pos.y = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.y, _saveNextPoint.y, enemyToPlayerPosFrame);
			_pos.z = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.z, _saveNextPoint.z, enemyToPlayerPosFrame);
		}
	}



	//１秒待ってから落下
	if (nowTime >= enemyRigidityTime + enemyToPlayerPosFrame + fallTime) {
		_pos.y -= _speed * 10;
		//とりあえずyが0になるまで落下
		if (_pos.y <= 0.0f) {
			_fallCount++;
			_pos.y = 0.0f;
			_easingFrame = 0;
			_currentTime = GetNowCount();
			VECTOR dirVec = VSub(_pos, _player->GetPosition());
			dirVec = VNorm(dirVec);
			_rotation.y = atan2(dirVec.x, dirVec.z);
			_savePos = _pos;
			_saveNextPoint = VAdd(_player->GetPosition(), VGet(0, 500, 0));
			ScreenVibration::GetInstance()->SetVibration(0, 20*(_fallCount+1), 20);
			if (_fallCount > 2) {
				_fallCount = 0;
				_modeState = ENEMYTYPE::COOLTIME;
			}
		}
	}
	return true;
};

bool SlaBlockPattern2::ModeCoolTime() {

	//プランナーさん側で変更できる場所　※秒数単位 
	float moveCoolTime = 3.0f * 1000; //攻撃してからのクールタイム   

	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_currentTime = 0;
		_modeState = ENEMYTYPE::DISCOVER;
	}
	return true;
};

bool SlaBlockPattern2::SetGravity() {
	//重力処理
	if (_modeState != ENEMYTYPE::ATTACK) {
		if (_pos.y > 0) {
			_gravity++;
			_pos.y -= _gravity;
			if (_pos.y < 0) {
				_gravity = 0;
				_pos.y = 0;
			}
		}
	}
	return true;
}

bool SlaBlockPattern2::DebugRender() {
	DrawSphere3D(VAdd(_pos, _diffeToCenter), _r, 16, GetColor(0, 0, 255), GetColor(0, 0, 255), false);
	return true;
}