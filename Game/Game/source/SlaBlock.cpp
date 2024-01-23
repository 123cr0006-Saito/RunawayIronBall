#include "SlaBlock.h"

const float SlaBlock::_fixSartchSize = 3000.0f;
const float SlaBlock::_fixDiscoverSize = 10000.0f;
const float SlaBlock::_fixAttackSize = 500.0f;

SlaBlock::SlaBlock(int model, VECTOR pos, Player* Player) :EnemyBase::EnemyBase(model, pos)
{
	//デバック時登録
	_model = MV1LoadModel("res/katatumuri/snail.mv1");
	//今のモデルに貼り付けているテクスチャ
	MV1SetTextureGraphHandle(_model, 0, ResourceServer::LoadGraph("res/katatumuri/14086_Snail_with_toy_house_for_ shell_v2_diff2.jpg"), true);
	MV1SetTextureGraphHandle(_model, 1, ResourceServer::LoadGraph("res/katatumuri/14086_Snail_with_toy_house_for_ shell_v2_diff.jpg"), true);

	_sartchRangeSize = _fixSartchSize;
	_discoverRangeSize = _fixDiscoverSize;
	_attackRangeSize = _fixAttackSize;

	//個別でセットするもの
	_player = Player;
	_sartchRange = _sartchRangeSize;
	_moveRange = 1000.0f;
	_speed = 5.0f;
	//_r = 100.0f;

	MV1SetScale(_model, VGet(0.1f, 0.1f, 0.1f));//持ってきたモデルが大きかったため1/10に設定
};

SlaBlock::~SlaBlock() {
	EnemyBase::~EnemyBase();
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
		_pos.y -= _speed * 8;
		//とりあえずyが0になるまで落下
		if (_pos.y <= 0.0f) {
			_pos.y = 0.0f;
			_easingFrame = 0;
			//global.effect->SetVibration(0, 20, 20);
			_currentTime = GetNowCount();
			_state = TYPE::cooltime;
		}
	}
	return true;
};

bool SlaBlock::ModeCoolTime() {

	//プランナーさん側で変更できる場所　※秒数単位 
	float moveCoolTime = 3.0f * 1000; //攻撃してからのクールタイム   

	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_currentTime = 0;
		_state = TYPE::discover;
	}
	return true;
};