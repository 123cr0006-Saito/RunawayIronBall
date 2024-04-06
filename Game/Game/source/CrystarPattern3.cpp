//----------------------------------------------------------------------
// @filename CrystarPattern3.h
// ＠date: 2024/01/26
// ＠author: saito ko
// @explanation
// 一番強いクライスターのパターン3(鉄)のクラス 回転しながら追尾攻撃をします。
//----------------------------------------------------------------------
#include "CrystarPattern3.h"

int CrystarPattern3::_collisionFrame = -1;
//----------------------------------------------------------------------
// @brief コンストラクタ
// @return なし
//----------------------------------------------------------------------
CrystarPattern3::CrystarPattern3() :EnemyBase::EnemyBase() {};
//----------------------------------------------------------------------
// @brief デストラクタ
// @return なし
//----------------------------------------------------------------------
CrystarPattern3::~CrystarPattern3() {
	delete _frameData;
	delete _animManager;
	for (int i = 0; i < 2; i++) {
		delete _roof[i];
	}
};
//----------------------------------------------------------------------
// @brief エネミーの固有変数の初期化
// @return なし
//----------------------------------------------------------------------
void CrystarPattern3::InheritanceInit() {
	//個別でセットするもの
	_animState = ANIMSTATE::IDLE;
	_attackPos = VGet(0, 0, 0);
	_attackDir = 0.0f;
};
//----------------------------------------------------------------------
// @brief アニメーションマネージャーとフレームデータ・クライスターの屋根の初期化
// @return なし
//----------------------------------------------------------------------
void CrystarPattern3::AnimInit() {

	_roof[0] = NEW CrystarRoof(ResourceServer::MV1LoadModel("CrystarRoof_Glass", "res/Enemy/Cg_Enemy_Crystar_Iron/Cg_Crystar_Roof_Iron.mv1"), _model,"joint1");
	_roof[1] = NEW CrystarRoof(ResourceServer::MV1LoadModel("CrystarRoof_Iron", "res/Enemy/Cg_Enemy_Crystar_Iron/Cg_Helmet_Enemy_Crystar_Iron.mv1"), _model, "joint2");

	//// モーションリストのロード
	MotionList::Load("Crystarl", "MotionList_Crystarl.csv");
	auto motionList = MotionList::GetMotionList("Crystarl");
	// アニメーションマネージャーの初期化
	_animManager = NEW AnimationManager();
	_animManager->InitMap("Crystarl", _model, *motionList);
	// フレームデータの初期化
	_frameData = NEW FrameData();
	_frameData->LoadData("Crystarl", *motionList);

	if (_collisionFrame == -1) {
		_collisionFrame = MV1SearchFrame(_model, "Hip");
	}

}
//----------------------------------------------------------------------
// @brief フレームデータでのコマンド処理
// @return 無し
//----------------------------------------------------------------------
void CrystarPattern3::CommandProcess() {
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
// @brief 初期化処理
// @param pos 位置
// @return 無し
//----------------------------------------------------------------------
void CrystarPattern3::Init(VECTOR pos) {
	_IsUse = true;

	SetKindPos(pos);
	_hp = _maxHp;
	_knockBackSpeedFrame = 0;
	_gravity = 0;
	_animState = ANIMSTATE::IDLE;
	_modeState = ENEMYTYPE::SEARCH;
	_searchState = SEARCHTYPE::COOLTIME;
	_rotation = VGet(0, 0, 0);

	float randSize = (float)(rand() % 75) / 100 + 0.75;// 1 + 0.0 ~ 0.5
	MV1SetScale(_model, VScale(VGet(1.0f, 1.0f, 1.0f), 2.0f * randSize));

	_diffeToCenter = VGet(0, 20.0f * 2.0f * randSize, 0);
	_r = 25.0f * 2.0f * randSize;
	_weightExp = _weightExp * 2.0f * randSize;

	InheritanceInit();
};
//----------------------------------------------------------------------
// @brief サーチ状態の更新処理
// ＠param plAttack プレイヤーが攻撃しているかどうか
// @return 成功したかどうか
//----------------------------------------------------------------------
bool CrystarPattern3::ModeSearch(bool plAttack) {
	switch (_searchState) {
	case SEARCHTYPE::MOVE:
		ModeSearchToMove();
		_animState = ANIMSTATE::RUN;
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
			_modeState = ENEMYTYPE::ATTACK;//状態を発見にする
			_animState = ANIMSTATE::HANDSTAND;
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
			// プレイヤーが視界に入ったら発見
			if (range_dir <= _flontAngle) {
				_modeState = ENEMYTYPE::ATTACK;//状態を発見にする
				_animState = ANIMSTATE::HANDSTAND;
				_currentTime = GetNowCount();
			}
		}
	}


	return true;
}
//----------------------------------------------------------------------
// @brief 攻撃状態の処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool CrystarPattern3::ModeAttack() {
	int nowTime = GetNowCount() - _currentTime;//今の状態になってから何秒経ったか？

	// 攻撃開始から2秒以内は動かない
	if (nowTime <= 1.5f * 1000)return true;

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
	if (p_distance >= _discoverRangeSize) {
		_modeState = ENEMYTYPE::SEARCH;//状態を索敵にする
		_animState = ANIMSTATE::IDLE;
		_orignPos = _nextMovePoint = _pos;
		_attackDir = 0.0f;
		_attackPos = VGet(0, 0, 0);
	}

	return true;
};
//----------------------------------------------------------------------
// @brief クールタイム時の処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool CrystarPattern3::ModeCoolTime() {
	
	if (GetNowCount() - _currentTime >= _coolTime) {
		_attackDir = 0.0f;
		_currentTime = GetNowCount();
		_animState = ANIMSTATE::HANDSTAND;
		_modeState = ENEMYTYPE::ATTACK;
	}
	return true;
};
//----------------------------------------------------------------------
// @brief ノックバック状態の処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool CrystarPattern3::ModeKnockBack() {
	int nowTime = GetNowCount() - _currentTime;
	float CoolTime = 3.0f * 1000; //硬直時間
	VECTOR knockBackVecter = VScale(_knockBackDir, _knockBackSpeedFrame);
	_pos = VAdd(_pos, knockBackVecter);
	if (_knockBackSpeedFrame > 0) {
		_knockBackSpeedFrame--;
	}
	if (_knockBackSpeedFrame <= 0 && nowTime > CoolTime) {
		_currentTime = GetNowCount();
		_animState = ANIMSTATE::HANDSTAND;
		_modeState = ENEMYTYPE::ATTACK;
	}
	return true;
};
//----------------------------------------------------------------------
// @brief このクラスの固有の処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool CrystarPattern3::IndividualProcessing() {
	for(int i = 0; i < 2; i++){
	_roof[i]->Update();
	}
	_animManager->Process(static_cast<int>(_animState));
	_frameData->Process(static_cast<int>(_animState), _animManager->GetPlayTime());
	return true;
}
//----------------------------------------------------------------------
// @brief モデルの更新処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool CrystarPattern3::SetState() {
	//最終的なモデルの位置や角度を調整
	if (_model != 0) {
		MV1SetRotationXYZ(_model, VGet(0.0f, _rotation.y + _attackDir, 0.0f));
		MV1SetPosition(_model, _pos);
	}
	return true;
};
//----------------------------------------------------------------------
// @brief 固有の描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool CrystarPattern3::IndividualRendering() {
	for (int i = 0; i < 2; i++) {
		_roof[i]->Render();
	}
	return true;
};
//----------------------------------------------------------------------
// @brief デバック用の描画処理
// @return 成功したかどうか
//----------------------------------------------------------------------
bool CrystarPattern3::DebugRender() {
	DrawSphere3D(MV1GetFramePosition(_model, _collisionFrame), _r, 8, GetColor(0, 255, 0), GetColor(0, 0, 255), false);
	return true;
};