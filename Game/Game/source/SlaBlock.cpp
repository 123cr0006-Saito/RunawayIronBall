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
	_animState = ANIMSTATE::IDLE;
};

void SlaBlock::AnimInit() {
	//// モーションリストのロード
	MotionList::Load("Slablock", "MotionList_Slablock.csv");
	auto motionList = MotionList::GetMotionList("Slablock");
	// アニメーションマネージャーの初期化
	_animManager = new AnimationManager();
	_animManager->InitMap("Slablock", _model, *motionList);
	// フレームデータの初期化
	_frameData = new FrameData();
	_frameData->LoadData("Slablock", *motionList);
}

void SlaBlock::CommandProcess() {
	std::vector<CommandParam> commandParam = _frameData->GetCommandData();

	for (auto itr = commandParam.begin(); itr != commandParam.end(); ++itr) {
		// コマンドを取得
		int command = itr->first;
		// パラメータを取得
		float param = itr->second;

		switch (command) {
		case EN_MOTION_CHANGE:
			_animState = static_cast<ANIMSTATE>(param);
			break;
		}
	}
};

bool SlaBlock::ModeSearch() {
	switch (_searchState) {
	case SEARCHTYPE::MOVE:
		ModeSearchToMove();
		_animState = ANIMSTATE::WALK;
		break;
	case SEARCHTYPE::TURN:
		ModeSearchToTurn();
		_animState = ANIMSTATE::WALK;
		break;
	case SEARCHTYPE::COOLTIME:
		ModeSearchToCoolTime();
		_animState = ANIMSTATE::IDLE;
		break;
	}

	//索敵処理
	VECTOR v_length = VSub(_player->GetCollision().down_pos, _pos);
	float len = VSize(v_length);
	if (VSize(v_length) <= _sartchRange) {

		MATRIX matrix = Math::MMultXYZ(0.0f, _rotation.y, 0.0f);
		VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
		VECTOR pla_dir = VNorm(v_length);
		float range_dir = Math::CalcVectorAngle(ene_dir, pla_dir);

		if (range_dir <= _flontAngle) {
			_modeState = ENEMYTYPE::DISCOVER;//状態を発見にする
			_sartchRange = _discoverRangeSize;//索敵範囲を発見時の半径に変更
			_currentTime = 0;
		}
	}

	return true;
}

bool SlaBlock::ModeDisCover() {
	//移動処理
	VECTOR move = VSub(_player->GetCollision().down_pos, _pos); move.y = 0.0f;//これをオンにするとy軸の移動がなくなる
	move = VNorm(move);
	move = VScale(move, _speed);
	_pos = VAdd(_pos, move);

	//移動方向に向きを変える
	VECTOR dirVec = VScale(move, -1);//方向ベクトルからモデルが向く方向を計算
	_rotation.y = atan2(dirVec.x, dirVec.z);

	//敵とプレイヤーの距離を算出
	move = VSub(_player->GetCollision().down_pos, _pos);
	float p_distance = VSize(move);//敵とプレイヤーの距離

	//索敵処理
	if (p_distance >= _sartchRange) {
		_animState = ANIMSTATE::IDLE;
		_modeState = ENEMYTYPE::SEARCH;//状態を索敵にする
		_sartchRange = _hearingRangeSize;//索敵範囲を発見時の半径に変更
		_orignPos = _nextMovePoint = _pos;
	}

	//攻撃処理
	if (p_distance <= _attackRangeSize) {
		_animState = ANIMSTATE::DROP;
		_modeState = ENEMYTYPE::ATTACK;//状態を索敵にする
		_currentTime = GetNowCount();
		_saveNextPoint = VAdd(_player->GetCollision().down_pos, VGet(0, 200, 0));
		_savePos = _pos;
	}
	return true;
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
			ScreenVibration::GetInstance()->SetVibration(0, 10, 10);
			_currentTime = GetNowCount();
			_animState = ANIMSTATE::STAN;
			_modeState = ENEMYTYPE::COOLTIME;
		}
	}
	return true;
};

bool SlaBlock::ModeCoolTime() {

	//プランナーさん側で変更できる場所　※秒数単位 
	float moveCoolTime = 3.0f * 1000; //攻撃してからのクールタイム   

	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_currentTime = 0;
		_animState = ANIMSTATE::WALK;
		_modeState = ENEMYTYPE::DISCOVER;
	}
	return true;
};

bool SlaBlock::ModeKnockBack() {
	VECTOR knockBackVecter = VScale(_knockBackDir, _knockBackSpeedFrame);
	_pos = VAdd(_pos, knockBackVecter);
	_knockBackSpeedFrame--;
	if (_knockBackSpeedFrame <= 0) {
		_animState = ANIMSTATE::WALK;
		_modeState = ENEMYTYPE::DISCOVER;
	}
	return true;
}

bool SlaBlock::SetGravity() {
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

bool SlaBlock::IndividualProcessing(){
	_animManager->Process(static_cast<int>(_animState));
	_frameData->Process(static_cast<int>(_animState), _animManager->GetPlayTime());
	return true;
};

bool SlaBlock::IndividualRendering() {
	return true;
};