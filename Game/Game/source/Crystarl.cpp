#include "Crystarl.h"

Crystarl::Crystarl() :EnemyBase::EnemyBase() {

};

Crystarl::~Crystarl() {
	EnemyBase::~EnemyBase();
};

void Crystarl::InheritanceInit() {
	//個別でセットするもの
	_player = Player::GetInstance();
	_r = 100.0f;
	_attackPos = VGet(0, 0, 0);
	_attackDir = 0.0f;
};

bool Crystarl::ModeSearch() {
	//-------------------------------
	// この敵はサーチモードの後追跡状態に入らず、攻撃に入る
	//------------------------------
	//移動方向の設定
	if (StopPos()) {
		if (_stopTime == 0) {
			_stopTime = (float)(rand() % 200) / 100.0f + 2.0f;//1秒から3秒まで止まる　小数点２桁までのランダム
			_currentTime = GetNowCount();
		}

		if ((float)(GetNowCount() - _currentTime) / 1000 >= _stopTime) {
			if (_nextDir == 0.0f) {

				VECTOR vArrow = VGet((float)(rand() % 20 / 10.0f) - 1.0f, 1.0f, (float)(rand() % 20 / 10.0f) - 1.0f);//ランダムな方向ベクトルを取る
				vArrow = VScale(vArrow, rand() % (int)_moveRange); vArrow.y = 0.0f;//基準点からの半径分をランダムで掛け、次に進むポイントを決める
				_saveNextPoint = VAdd(vArrow, _orignPos);//基準点に平行移動

				VECTOR dirVec = VSub(_saveNextPoint, _pos);//方向ベクトルからモデルが向く方向を計算
				dirVec = VNorm(dirVec);
				MATRIX matrix = Math::MMultXYZ(0.0f, _direction, 0.0f);
				VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
				float range_dir = Math::CalcVectorAngle(ene_dir, dirVec);
				VECTOR arrow = VCross(ene_dir, dirVec);
				if (arrow.y < 0) {
					range_dir *= -1;
				}
				_nextDir = _direction + range_dir;
				_oldDir = _direction;
			}
			else {
				_easingFrame++;
				_direction = Easing::Linear(_easingFrame, _oldDir, _nextDir, 60);
				if (_easingFrame >= 60) {
					_easingFrame = 0;
					_nextDir = 0.0f;
					_stopTime = 0;//時間の初期化
					_nextMovePoint = _saveNextPoint;
				}
			}
		}
	}
	else {
		//移動処理
		VECTOR move = VSub(_nextMovePoint, _pos);
		move = VNorm(move);
		move = VScale(move, _speed);
		_pos = VAdd(_pos, move);
	}
	
	//索敵処理
	VECTOR v_length = VSub(_player->GetCollision().down_pos, _pos);
	float len = VSize(v_length);
	if (VSize(v_length) <= _sartchRange) {

		MATRIX matrix = Math::MMultXYZ(0.0f, _direction, 0.0f);
		VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
		VECTOR pla_dir = VNorm(v_length);
		float range_dir = Math::CalcVectorAngle(ene_dir, pla_dir);

		if (range_dir <= _flontAngle) {
			_state = ENEMYTYPE::ATTACK;//状態を発見にする
			_sartchRange = _discoverRangeSize;//索敵範囲を発見時の半径に変更
			_currentTime = GetNowCount();
			_stopTime = 0;
		}
	}

	return true;
}

bool Crystarl::ModeAttack() {
	int nowTime = GetNowCount() - _currentTime;//今の状態になってから何秒経ったか？

	//今の問題　プレイヤーのほうを常に向くので急激に角度が変化したとき逆位置に移動する

	//プランナーさん側で変更できる場所　※秒数単位 
	int rottime = 1.0f * 1000.0f;//攻撃までの回転時間
	int oneLoundTime = 3.0f * 1000.0f;//左右の揺れに対する一周する時間
	int _rotAttackMaxDistance = 10.0f;//攻撃時の横に移動する最大距離

	//VECTOR move = VSub(_player->GetPos(),VAdd( _pos, _attackPos)); move.y = 0.0f;//これをオンにするとy軸の移動がなくなる
	VECTOR move = VSub(_player->GetCollision().down_pos,_pos); move.y = 0.0f;//これをオンにするとy軸の移動がなくなる
	move = VNorm(move);
	move = VScale(move, _speed);
	_pos = VAdd(_pos, move);

	//移動方向に向きを変える
	VECTOR dirVec = VScale(move, -1);//方向ベクトルからモデルが向く方向を計算
	_direction = atan2(dirVec.x, dirVec.z);

	_attackDir += Math::DegToRad(30.0f);//毎フレーム30度増やす

	MATRIX matrix = Math::MMultXYZ(0.0f, _direction, 0.0f);
	VECTOR xDirVec = Math::MatrixToVector(matrix, 0);
	_attackPos = VScale(xDirVec, _rotAttackMaxDistance * sin(DX_PI_F * 2.0f * ((float)nowTime / oneLoundTime)));
	_pos = VAdd(_pos, _attackPos);
	//敵とプレイヤーの距離を算出
	//move = VSub(_player->GetPos(), VAdd(_pos, _attackPos));
	move = VSub(_player->GetCollision().down_pos, _pos);
	float p_distance = VSize(move);//敵とプレイヤーの距離

	//索敵処理
	if (p_distance >= _sartchRange) {
		_state = ENEMYTYPE::SEARCH;//状態を索敵にする
		_sartchRange = _hearingRangeSize;//索敵範囲を発見時の半径に変更
	//	_orignPos = _nextMovePoint = VAdd(_pos, _attackPos);
		_orignPos = _nextMovePoint = _pos;
		_attackDir = 0.0f;
		_attackPos = VGet(0, 0, 0);
	}

	return true;
};

bool Crystarl::ModeCoolTime() {
	//プランナーさん側で変更できる場所　※秒数単位 
	float moveCoolTime = 2.0f * 1000; //攻撃してからのクールタイム   
	
	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_attackDir = 0.0f;
		_currentTime = GetNowCount();
		_state = ENEMYTYPE::ATTACK;
	}
	return true;
};

bool Crystarl::SetState() {
	//最終的なモデルの位置や角度を調整
	if (_model != 0) {
		MV1SetRotationXYZ(_model, VGet(0.0f, _direction + _attackDir, 0.0f));
		//MV1SetPosition(_model, VAdd(_pos, _attackPos));
		MV1SetPosition(_model, _pos);
	}
	return true;
};

bool Crystarl::DebugRender() {
	DrawSphere3D(VAdd(VAdd(_pos, _diffeToCenter), _attackPos), _r, 32, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
	return true;
};

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CrystarlPattern2::CrystarlPattern2() :EnemyBase::EnemyBase() {

};

CrystarlPattern2::~CrystarlPattern2() {
	EnemyBase::~EnemyBase();
};

void CrystarlPattern2::InheritanceInit() {

	//個別でセットするもの
	_player = Player::GetInstance();
	_r = 100.0f;

	_attackPos = VGet(0, 0, 0);
	_attackDir = 0.0f;
	_nowAttackDistance = 0;
	_attackDistanceSpeed = 3;
};

bool CrystarlPattern2::ModeAttack() {
	int nowTime = GetNowCount() - _currentTime;//今の状態になってから何秒経ったか？

	//プランナーさん側で変更できる場所　※秒数単位 
	int rottime = 1.0f * 1000.0f;//攻撃までの回転時間
	int oneLoundTime = 0.7f * 1000.0f;//左右の揺れに対する一周する時間
	int attackRange = 750;//攻撃範囲の半径 cm

	_attackDir -= Math::DegToRad(30.0f);//毎フレーム30度増やす

	if (nowTime >= rottime) {
		//1周する前に半径の伸びが-になるとアルキメデスの法則から外れるので螺旋ではなく正面に飛び出た形になる
		_nowAttackDistance += _attackDistanceSpeed;
		if (_nowAttackDistance <= 0 || attackRange <= _nowAttackDistance) {
			_attackDistanceSpeed = -_attackDistanceSpeed;
			if (_nowAttackDistance <= 0) {
				_state = ENEMYTYPE::COOLTIME;
				_currentTime = GetNowCount();
				_nowAttackDistance = 0;
			}
		}
	}
	_attackPos.z = _nowAttackDistance * sin(DX_PI_F * 2.0f * ((float)nowTime / oneLoundTime));
	_attackPos.x = _nowAttackDistance * cos(DX_PI_F * 2.0f * ((float)nowTime / oneLoundTime));

	return true;
};

bool CrystarlPattern2::ModeCoolTime() {
	//プランナーさん側で変更できる場所　※秒数単位 
	float moveCoolTime = 2.0f * 1000; //攻撃してからのクールタイム   
	_attackDir += Math::DegToRad(15.0f);//毎フレーム30度増やす
	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_attackDir = 0.0f;
		_currentTime = GetNowCount();
		_state = ENEMYTYPE::DISCOVER;
	}
	return true;
};

bool CrystarlPattern2::SetState() {
	//最終的なモデルの位置や角度を調整
	if (_model != 0) {
		MV1SetRotationXYZ(_model, VGet(0.0f, _direction + _attackDir, 0.0f));
		MV1SetPosition(_model, VAdd(_pos, _attackPos));
	}
	return true;
};

bool CrystarlPattern2::DebugRender() {
	DrawSphere3D(VAdd(VAdd(_pos, _diffeToCenter), _attackPos), _r, 32, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
	return true;
};