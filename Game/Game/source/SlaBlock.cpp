#include "SlaBlock.h"

SlaBlock::SlaBlock() :EnemyBase::EnemyBase()
{
	//デバック時登録
	//_model = ResourceServer::MV1LoadModel("res/katatumuri/snail.mv1");
};

SlaBlock::~SlaBlock() {
	EnemyBase::~EnemyBase();
};

void SlaBlock::InheritanceInit() {

};

bool SlaBlock::ModeAttack() {
	int nowTime = GetNowCount() - _currentTime;//今の状態になってから何秒経ったか？

	//プランナーさん側で変更できる場所　※秒数単位 
	int enemyRigidityTime = 1.0f * 1000; //攻撃モーションに入っての硬直時間
	int enemyToPlayerPosFrame = 20.0f / 60.0f * 1000;//敵がプレイヤーの位置につくまでの時間
	int fallTime = 1.0f * 1000;//落下するまでの時間

	// 1秒待ってから20フレームでプレイヤーの頭上に到着
	if (enemyRigidityTime <= nowTime && nowTime < enemyRigidityTime + enemyToPlayerPosFrame) {
		_pos.x = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.x, _saveNextPoint.x, enemyToPlayerPosFrame);
		_pos.y = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.y, _saveNextPoint.y, enemyToPlayerPosFrame);
		_pos.z = Easing::InQuart(nowTime - enemyRigidityTime, _savePos.z, _saveNextPoint.z, enemyToPlayerPosFrame);
	}

	//１秒待ってから落下
	if (nowTime >= enemyRigidityTime + enemyToPlayerPosFrame + fallTime) {
		_pos.y -= _speed * 10;
		//とりあえずyが0になるまで落下
		if (_pos.y <= 0.0f) {
			_pos.y = 0.0f;
			_easingFrame = 0;
			ScreenVibration::GetInstance()->SetVibration(0, 20, 20);
			_currentTime = GetNowCount();
			_state = ENEMYTYPE::COOLTIME;
		}
	}
	return true;
};

bool SlaBlock::ModeCoolTime() {

	//プランナーさん側で変更できる場所　※秒数単位 
	float moveCoolTime = 3.0f * 1000; //攻撃してからのクールタイム   

	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_currentTime = 0;
		_state = ENEMYTYPE::DISCOVER;
	}
	return true;
};