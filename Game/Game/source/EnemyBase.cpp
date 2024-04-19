//----------------------------------------------------------------------
// @filename EnemyBase.cpp
// ＠date: 2023/12/14
// ＠author: saito ko
// @explanation
// エネミーの基本行動や共通の変数が書かれた基底クラス
//----------------------------------------------------------------------
#include "EnemyBase.h"
#include "EnemyManeger.h"
//----------------------------------------------------------------------
// @brief: コンストラクタ
// @return: 無し
//----------------------------------------------------------------------
EnemyBase::EnemyBase() : ObjectBase() {
	_player = nullptr;

	_stopTime = 0.0f;
	_currentTime = 0;

	_nextDir = 0.0f;
	_oldDir = 0.0f;

	_easingFrame = 0;
	_saveNextPoint = VGet(0, 0, 0);

	_cell->_objType = OBJ_TYPE::EN;
};
//----------------------------------------------------------------------
// @brief: デストラクタ
// @return: 無し
//----------------------------------------------------------------------
EnemyBase::~EnemyBase() {
	_player = nullptr;
};
//----------------------------------------------------------------------
// @brief: エネミーの初期化
// @param: model モデルハンドル
// @param: pos 初期座標
// @param: param パラメータ
// @param: name エネミーの名前
// @return: 無し
//----------------------------------------------------------------------
bool EnemyBase::Create(int model, VECTOR pos, EnemyParam param, std::string name) {
	_model = model;

	_player = Player::GetInstance();

	//Param------------------
	_hp = param._hp;
	_maxHp = _hp;
	_weightExp = param._exp;
	_speed = param._speed;
	_coolTime = param._coolTime;

	_flontAngle = param._flontAngle;
	_hearingRangeSize = param._hearingRangeSize;
	_moveRange = param._moveRange;
	_searchRange = param._searchRange;
	_discoverRangeSize = param._discoverRangeSize;
	_attackRangeSize = param._attackRangeSize;
	_suppression = param._suppression;

	Init(pos);
	InheritanceInit();
	
	MV1SetPosition(_model, _pos);

	AnimInit();

	return true;
};
//----------------------------------------------------------------------
// @brief: エネミーの初期化
// @param: pos 初期座標
// @param: scale エネミーの大きさ
// @return: 無し
//----------------------------------------------------------------------
void EnemyBase::Init(VECTOR pos, float scale) {};
//----------------------------------------------------------------------
// @brief: エネミーの初期化
// @param: pos 初期座標
// @return: 無し
//----------------------------------------------------------------------
void EnemyBase::Init(VECTOR pos) {
	_IsUse = true;

	SetKindPos(pos);
	_hp = _maxHp;
	_knockBackSpeedFrame = 0;
	_gravity = 0;
	_modeState = ENEMYTYPE::SEARCH;
	_searchState = SEARCHTYPE::COOLTIME;
	_rotation = VGet(0, 0, 0);
	_forwardVec = VScale(Math::MatrixToVector(MGetRotY(_rotation.y), 2), -1);// モデルが-ｚの方向を正面としているので-1をかける

	float randSize = (float)(rand() % 75) / 100 + 0.75;// 1 + 0.0 ~ 0.5
	MV1SetScale(_model, VScale(VGet(1.0f, 1.0f, 1.0f), 2 * randSize));//スラブロックの借りが小さかったため2倍に設定

	_diffeToCenter = VGet(0, 125 * randSize, 0);
	_r = 150.0f * randSize;
	_weightExp = _weightExp * randSize;

};
//----------------------------------------------------------------------
// @brief: 固有変数の初期化
// @return: 無し
//----------------------------------------------------------------------
void EnemyBase::InheritanceInit() {};
//----------------------------------------------------------------------
// @brief: アニメーションの初期化
// @return: 無し
//----------------------------------------------------------------------
void EnemyBase::AnimInit() {};
//----------------------------------------------------------------------
// @brief: エネミーの位置を設定
// @return: 無し
//----------------------------------------------------------------------
void EnemyBase::SetPos(VECTOR pos) {
	_pos = pos;
	_saveNextPoint = pos;
};
//----------------------------------------------------------------------
// @brief: エネミーの初期位置を設定
// @return: 無し
//----------------------------------------------------------------------
void EnemyBase::SetKindPos(VECTOR pos) {
	_pos = pos;
	_orignPos = pos;
	_savePos = pos;
	_nextMovePoint = pos;
};
//----------------------------------------------------------------------
// @brief: エネミーの回転処理
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EnemyBase::ModeSearchToTurn() {
	_easingFrame++;
	_rotation.y = Easing::Linear(_easingFrame, _oldDir, _nextDir, 60);
	if (_easingFrame >= 60) {
		_easingFrame = 0;
		_nextMovePoint = _saveNextPoint;
		_currentTime = GetNowCount();
		if (rand() % 4 == 0) {
			_stopTime = 1;
			_searchState = SEARCHTYPE::COOLTIME;
		}
		else {
			_stopTime = (float)(rand() % 200) / 100.0f + 2.0f;//2秒から4秒まで止まる　小数点２桁までのランダム
			_forwardVec = VScale(Math::MatrixToVector(MGetRotY(_rotation.y), 2),-1);// モデルが-ｚの方向を正面としているので-1をかける
			_searchState = SEARCHTYPE::MOVE;
		}
	}
	return true;
};
//----------------------------------------------------------------------
// @brief: エネミーの移動処理
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EnemyBase::ModeSearchToMove() {
	VECTOR move = VScale(_forwardVec, _speed);
	_pos = VAdd(_pos, move);

	if (GetNowCount() - _currentTime >= _stopTime * 1000) {
		_stopTime = (float)(rand() % 200) / 100.0f + 2.0f;//2秒から4秒まで止まる　小数点２桁までのランダム
		_currentTime = GetNowCount();
		_searchState = SEARCHTYPE::COOLTIME;
	}

	return true;
};
//----------------------------------------------------------------------
// @brief: エネミーの移動後の休憩処理
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EnemyBase::ModeSearchToCoolTime() {
	if (GetNowCount() - _currentTime >= _stopTime * 1000) {

		VECTOR vArrow = VGet((float)(rand() % 20 / 10.0f) - 1.0f, 1.0f, (float)(rand() % 20 / 10.0f) - 1.0f);//ランダムな方向ベクトルを取る
		vArrow = VScale(vArrow, rand() % (int)_moveRange); vArrow.y = 0.0f;//基準点からの半径分をランダムで掛け、次に進むポイントを決める
		_saveNextPoint = VAdd(vArrow, _orignPos);//基準点に平行移動

		VECTOR dirVec = VSub(_saveNextPoint, _pos);//方向ベクトルからモデルが向く方向を計算
		dirVec = VNorm(dirVec);
		MATRIX matrix = Math::MMultXYZ(0.0f, _rotation.y, 0.0f);
		VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
		float range_dir = Math::CalcVectorAngle(ene_dir, dirVec);
		VECTOR arrow = VCross(ene_dir, dirVec);
		if (arrow.y < 0) {
			range_dir *= -1;
		}
		_nextDir = _rotation.y + range_dir;
		_oldDir = _rotation.y;
		_stopTime = 0;
		_searchState = SEARCHTYPE::TURN;
	}
	return true;
};
//----------------------------------------------------------------------
// @brief: エネミーの索敵処理
// @param: plAttack プレイヤーが攻撃しているかどうか
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EnemyBase::ModeSearch(bool plAttack) {
	switch (_searchState) {
	case SEARCHTYPE::MOVE:
		ModeSearchToMove();
		break;
	case SEARCHTYPE::TURN:
		ModeSearchToTurn();
		break;
	case SEARCHTYPE::COOLTIME:
		ModeSearchToCoolTime();
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
// @brief: エネミーの追跡処理
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EnemyBase::ModeDisCover() {
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
	if (pl_distance >= _discoverRangeSize * _discoverRangeSize) {
		_modeState = ENEMYTYPE::SEARCH;//状態を索敵にする
		_orignPos = _nextMovePoint = _pos;
	}

	//攻撃処理
	if (pl_distance <= _attackRangeSize * _attackRangeSize) {
		_modeState = ENEMYTYPE::ATTACK;//状態を索敵にする
		_currentTime = GetNowCount();
		_saveNextPoint = VAdd(_player->GetCollision().down_pos, VGet(0, 500, 0));
		_savePos = _pos;
	}
	return true;
};
//----------------------------------------------------------------------
// @brief: エネミーの攻撃処理
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EnemyBase::ModeAttack() {
	return true;
};
//----------------------------------------------------------------------
// @brief: エネミーのクールタイム処理
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EnemyBase::ModeCoolTime() {
	return true;
};
//----------------------------------------------------------------------
// @brief: エネミーのノックバック処理
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EnemyBase::ModeKnockBack() {
	int nowTime = GetNowCount() - _currentTime;
	float CoolTime = 3.0f * 1000; //攻撃してからのクールタイム   
	VECTOR knockBackVecter = VScale(_knockBackDir, _knockBackSpeedFrame);
	_pos = VAdd(_pos, knockBackVecter);
	if (_knockBackSpeedFrame > 0) {
		_knockBackSpeedFrame--;
	}

	if (_knockBackSpeedFrame <= 0 && nowTime > CoolTime) {
		_modeState = ENEMYTYPE::DISCOVER;
	}

	return true;
};
//----------------------------------------------------------------------
// @brief: エネミーの死亡処理
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EnemyBase::ModeDead() {
	VECTOR knockBackVecter = VScale(_knockBackDir, _knockBackSpeedFrame);
	_pos = VAdd(_pos, knockBackVecter);
	_knockBackSpeedFrame--;
	if (_knockBackSpeedFrame <= 0) {
		_IsUse = false;
		//エネミーをコリジョン判定から削除
		_collisionManager->ReserveRemovementCell(_cell);
	}
	return true;
};
//----------------------------------------------------------------------
// @brief: エネミーの個別処理
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EnemyBase::IndividualProcessing() {
	return true;
}
//----------------------------------------------------------------------
// @brief: エネミーの位置設定
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EnemyBase::SetState() {
	//最終的なモデルの位置や角度を調整
	if (_model != 0) {
		MV1SetRotationXYZ(_model, VGet(0.0f, _rotation.y, 0.0f));
		MV1SetPosition(_model, _pos);
	}
	return true;
};
//----------------------------------------------------------------------
// @brief: エネミーの重力処理
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EnemyBase::SetGravity() {
	//重力処理
	_gravity++;
	_pos.y -= _gravity;
	if (_pos.y < 0) {
		_gravity = 0;
		_pos.y = 0;
	}
	return true;
};
//----------------------------------------------------------------------
// @brief: エネミーに攻撃時、ダメージとノックバックの設定
// @param: vDir 攻撃方向
// @param: damage ダメージ
// @return: 無し
//----------------------------------------------------------------------
void EnemyBase::SetKnockBackAndDamage(VECTOR vDir, float damage) {
	if (_knockBackSpeedFrame <= 0) {
		//初期化
		InheritanceInit();
		//ノックバック処理 
		//方向ベクトルからエネミーの向きをダメージを与えたものの方向に回転をする
		_rotation.y = atan2(vDir.x, vDir.z);
		_hp -= damage;
		_knockBackDir = vDir;
		_currentTime = GetNowCount();
		//エフェクトの生成
		VECTOR effectPos = VAdd(VAdd(_pos, _diffeToCenter), VScale(vDir, -50));
		int effectHandle[30];
		ResourceServer::LoadMultGraph("HitEffect_Blue", "res/Effect/HitEffect_Blue/HitEffect_Blue", ".png", 30, effectHandle);
		BoardPolygon* effect = NEW BoardPolygon(effectPos, GetCameraBillboardMatrix(), 200, effectHandle, 30, 0.5f / 60.0f * 1000.0f);
		EffectManeger::GetInstance()->LoadEffect(effect);

		//ノックバックの速度を計算
		int knockBackSpeedFrame = damage - _weightExp;
		//死亡時
		if (_hp <= 0) {
			//ノックバックの速度を制限
			_knockBackSpeedFrame = Math::Clamp(EN_KNOCKBACK_OF_DEAT_MIN, EN_KNOCKBACK_OF_DEAT_MAX, knockBackSpeedFrame);
			//制圧値を減算
			Suppression::GetInstance()->SubSuppression(_suppression);
			//プレイヤーの経験値を加算
			_player->SetExp(_weightExp);
			//エネミーを死亡状態にする
			_modeState = ENEMYTYPE::DEAD;
		}
		//生存時
		else {
		    //ノックバックの速度を制限
		    _knockBackSpeedFrame = Math::Clamp(EN_KNOCKBACK_MIN, EN_KNOCKBACK_MAX, knockBackSpeedFrame);
			//ノックバックモードに移行
		    _modeState = ENEMYTYPE::KNOCKBACK;
		}
	}
};
//----------------------------------------------------------------------
// @brief: フレームデータのコマンド処理
// @return: 無し
//----------------------------------------------------------------------
void EnemyBase::CommandProcess() {};
//----------------------------------------------------------------------
// @brief: エネミーの更新処理
// @param: plAttack プレイヤーが攻撃しているかどうか
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EnemyBase::Process(bool plAttack) {
	if (_IsUse) {
		//モード別処理
		switch (_modeState) {
		case ENEMYTYPE::SEARCH:
			ModeSearch(plAttack);
			break;
		case ENEMYTYPE::DISCOVER:
			ModeDisCover();
			break;
		case ENEMYTYPE::ATTACK:
			ModeAttack();
			break;
		case ENEMYTYPE::COOLTIME:
			ModeCoolTime();
			break;
		case ENEMYTYPE::KNOCKBACK:
			ModeKnockBack();
			break;
		case ENEMYTYPE::DEAD:
			ModeDead();
			break;
		}
		//重力処理
		SetGravity();
	
		//ノックバック中のけぞり処理 仮です
		if (_modeState == ENEMYTYPE::KNOCKBACK) {
			if (_pos.y > 0) {
				if (_rotation.x < Math::DegToRad(60)) {
					_rotation.x += Math::DegToRad(1.2f);
				}
			}
		}
		else {
			if (_rotation.x > Math::DegToRad(0)) {
				_rotation.x -= Math::DegToRad(2);
			}
		}
		//最終的なモデルの位置や角度を調整
		SetState();
		//個別処理
		IndividualProcessing();
		//コマンド処理
		CommandProcess();
	}

	return true;
};
//----------------------------------------------------------------------
// @brief: エネミーの描画処理
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool  EnemyBase::DebugRender() {
	DrawSphere3D(VAdd(_pos, _diffeToCenter), _r, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
	return true;
};
//----------------------------------------------------------------------
// @brief: エネミーの固有の描画処理
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EnemyBase::IndividualRendering() {
	return true;
};
//----------------------------------------------------------------------
// @brief: エネミーの描画処理
// @return: 成功したかどうか
//----------------------------------------------------------------------
bool EnemyBase::Render() {
	if (_model != 0) {   
#ifdef _DEBUG
		DebugRender();
#endif
		MV1DrawModel(_model);
		IndividualRendering();
	}
	return true;
};

