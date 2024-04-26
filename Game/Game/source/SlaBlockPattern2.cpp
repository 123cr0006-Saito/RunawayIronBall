//----------------------------------------------------------------------
// @filename SlaBlockPattern2.cpp
// @date: 2024/01/26
// @author: saito ko
// @explanation
// スラブロックの攻撃パターン2(ガラス,鉄)のクラス 1度だけプレイヤーの真上から落下攻撃を行います
//----------------------------------------------------------------------
#include "SlaBlockPattern2.h"

int SlaBlockPattern2::_collisionFrame = -1;
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return 無し
//----------------------------------------------------------------------
SlaBlockPattern2::SlaBlockPattern2() :EnemyBase::EnemyBase(){};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
SlaBlockPattern2::~SlaBlockPattern2() {
	delete _animManager;
	delete _frameData;
};
//----------------------------------------------------------------------
// @brief エネミーの固有変数の初期化
// @return 成功しているか
//----------------------------------------------------------------------
void SlaBlockPattern2::InheritanceInit() {
	_fallCount = 0;
};
//----------------------------------------------------------------------
// @brief アニメーション・フレームデータの初期化
// @return 無し
//----------------------------------------------------------------------
void SlaBlockPattern2::AnimInit() {
	//// モーションリストのロード
	MotionList::Load("Slablock", "MotionList_Slablock.csv");
	auto motionList = MotionList::GetMotionList("Slablock");
	// アニメーションマネージャーの初期化
	_animManager = NEW AnimationManager();
	_animManager->InitMap("Slablock", _model, *motionList);
	// フレームデータの初期化
	_frameData = NEW FrameData();
	_frameData->LoadData("Slablock", *motionList);

	if (_collisionFrame == -1) {
		_collisionFrame = MV1SearchFrame(_model, "face1");
	}
}
//----------------------------------------------------------------------
// @brief  フレームデータでのコマンド処理
// @return 無し
//----------------------------------------------------------------------
void SlaBlockPattern2::CommandProcess() {
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
//----------------------------------------------------------------------
// @brief  サーチ状態の処理
// @param plAttack : プレイヤーが攻撃しているかどうか
// @return 成功しているかどうか
//----------------------------------------------------------------------
bool SlaBlockPattern2::ModeSearch(bool plAttack){
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
	VECTOR dirVec = VSub(_player->GetCollision().down_pos, _pos);
	float length = VSquareSize(dirVec);
	if (plAttack) {
		// プレイヤーが攻撃時は聴覚範囲で探索
		if (length <= _hearingRangeSize * _hearingRangeSize) {
			_modeState = ENEMYTYPE::DISCOVER;//状態を発見にする
			_currentTime = GetNowCount();
		}
	}
	else {
		// プレイヤーが攻撃していないときは視界での検索
		if (length <= _searchRange * _searchRange) {

			MATRIX matrix = Math::MMultXYZ(0.0f, _rotation.y, 0.0f);
			VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
			VECTOR pla_dir = VNorm(dirVec);
			float range_dir = Math::CalcVectorAngle(ene_dir, pla_dir);

			if (range_dir <= _flontAngle) {
				_modeState = ENEMYTYPE::DISCOVER;//状態を発見にする
				_currentTime = GetNowCount();
			}
		}
	}


	return true;
};
//----------------------------------------------------------------------
// @brief 追跡状態の処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool SlaBlockPattern2::ModeDisCover() {
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
	float pl_distance = VSquareSize(move);//敵とプレイヤーの距離

	//索敵処理
	if (pl_distance >= _searchRange * _searchRange) {
		_modeState = ENEMYTYPE::SEARCH;//状態を索敵にする
		_animState = ANIMSTATE::IDLE;
		_orignPos = _nextMovePoint = _pos;
	}

	//攻撃処理
	if (pl_distance <= _attackRangeSize * _attackRangeSize) {
		_modeState = ENEMYTYPE::ATTACK;//状態を索敵にする
		_animState = ANIMSTATE::DROP;
		_currentTime = GetNowCount();
		_saveNextPoint = VAdd(_player->GetCollision().down_pos, VGet(0, 500, 0));
		_savePos = _pos;
	}
	return true;
};
//----------------------------------------------------------------------
// @brief 攻撃状態の処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool SlaBlockPattern2::ModeAttack() {
	int nowTime = GetNowCount() - _currentTime;//今の状態になってから何秒経ったか？

	//プランナーさん側で変更できる場所　※秒数単位 
	int enemyRigidityTime = 1.0f * 1000; //攻撃モーションに入っての硬直時間
	int enemyToPlayerPosFrame = 20.0f / 60.0f * 1000;//敵がプレイヤーの位置につくまでの時間
	int fallTime = 1.0f * 1000;//落下するまでの時間
	float fallSpeed = 30.0f;

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
		_pos.y -= fallSpeed;
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
			_saveNextPoint = VAdd(_player->GetPosition(), VGet(0, 200, 0));
			_animState = ANIMSTATE::DROP;
			ScreenVibration::GetInstance()->SetVibration(0, 5*(_fallCount+1), 10);
			if (_fallCount > 2) {
				_fallCount = 0;
				_animState = ANIMSTATE::STAN;
				_modeState = ENEMYTYPE::COOLTIME;
			}
		}
	}
	return true;
};
//----------------------------------------------------------------------
// @brief クールタイム時の処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool SlaBlockPattern2::ModeCoolTime() {

	if (GetNowCount() - _currentTime >= _coolTime) {
		_currentTime = 0;
		_animState = ANIMSTATE::WALK;
		_modeState = ENEMYTYPE::DISCOVER;
	}
	return true;
};
//----------------------------------------------------------------------
// @brief ノックバック状態の処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool SlaBlockPattern2::ModeKnockBack() {
	int nowTime = GetNowCount() - _currentTime;
	float CoolTime = 3.0f * 1000; //硬直時間
	VECTOR knockBackVecter = VScale(_knockBackDir, _knockBackSpeedFrame);
	_pos = VAdd(_pos, knockBackVecter);
	if (_knockBackSpeedFrame > 0) {
		_knockBackSpeedFrame--;
	}
	if (_knockBackSpeedFrame <= 0 && nowTime > CoolTime) {
		_animState = ANIMSTATE::WALK;
		_modeState = ENEMYTYPE::DISCOVER;
	}
	return true;
};
//----------------------------------------------------------------------
// @brief 重力の設定
// @return 成功したかどうか
//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
// @brief このクラスの固有の処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool SlaBlockPattern2::IndividualProcessing(){
	_animManager->Process(static_cast<int>(_animState));
	_frameData->Process(static_cast<int>(_animState), _animManager->GetPlayTime());
	return true;
};
//----------------------------------------------------------------------
// @brief このクラスの固有の描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool SlaBlockPattern2::IndividualRendering() {
	return true;
};
//----------------------------------------------------------------------
// @brief デバッグ用の描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool SlaBlockPattern2::DebugRender() {
	DrawSphere3D(MV1GetFramePosition(_model, _collisionFrame), _r, 8, GetColor(0, 0, 255), GetColor(0, 0, 255), false);
	return true;
}