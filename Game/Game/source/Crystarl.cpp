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
				MATRIX matrix = Math::MMultXYZ(0.0f, _rotation.y, 0.0f);
				VECTOR ene_dir = VScale(Math::MatrixToVector(matrix, 2), -1);
				float range_dir = Math::CalcVectorAngle(ene_dir, dirVec);
				VECTOR arrow = VCross(ene_dir, dirVec);
				if (arrow.y < 0) {
					range_dir *= -1;
				}
				_nextDir = _rotation.y + range_dir;
				_oldDir = _rotation.y;
			}
			else {
				_easingFrame++;
				_rotation.y = Easing::Linear(_easingFrame, _oldDir, _nextDir, 60);
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

		MATRIX matrix = Math::MMultXYZ(0.0f, _rotation.y, 0.0f);
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
	_rotation.y = atan2(dirVec.x, dirVec.z);

	_attackDir += Math::DegToRad(30.0f);//毎フレーム30度増やす

	MATRIX matrix = Math::MMultXYZ(0.0f, _rotation.y, 0.0f);
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

bool Crystarl::ModeKnockBack() {
	VECTOR knockBackVecter = VScale(_knockBackDir, _knockBackSpeedFrame);
	_pos = VAdd(_pos, knockBackVecter);
	_knockBackSpeedFrame--;
	if (_knockBackSpeedFrame <= 0) {
		_state = ENEMYTYPE::ATTACK;
	}
	return true;
};

bool Crystarl::SetState() {
	//最終的なモデルの位置や角度を調整
	if (_model != 0) {
		MV1SetRotationXYZ(_model, VGet(0.0f, _rotation.y + _attackDir, 0.0f));
		//MV1SetPosition(_model, VAdd(_pos, _attackPos));
		MV1SetPosition(_model, _pos);
	}
	return true;
};

bool Crystarl::DebugRender() {
	DrawSphere3D(VAdd(VAdd(_pos, _diffeToCenter), _attackPos), _r, 32, GetColor(0, 255, 0), GetColor(0, 0, 255), false);
	return true;
};