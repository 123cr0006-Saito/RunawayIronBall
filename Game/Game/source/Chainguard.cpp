#include "Chainguard.h"
Chainguard::Chainguard() {

};

Chainguard::~Chainguard(){
	delete _animManager;
	delete _frameData;
};

void Chainguard::InheritanceInit(){
	_animState = ANIMSTATE::IDLE;
};

void Chainguard::AnimInit(){
	// モーションリストのロード
	MotionList::Load("Chainguard", "MotionList_Chainguard.csv");
	auto motionList = MotionList::GetMotionList("Chainguard");
	// アニメーションマネージャーの初期化
	_animManager = new AnimationManager();
	_animManager->InitMap("Chainguard", _model, *motionList);
	// フレームデータの初期化
	_frameData = new FrameData();
	_frameData->LoadData("Chainguard", *motionList);
};

void Chainguard::CommandProcess(){
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

bool Chainguard::ModeSearch(){
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
};

bool Chainguard::ModeAttack(){
	return true;
};

bool Chainguard::ModeCoolTime(){
	//プランナーさん側で変更できる場所　※秒数単位 
	float moveCoolTime = 3.0f * 1000; //攻撃してからのクールタイム   

	if (GetNowCount() - _currentTime >= moveCoolTime) {
		_currentTime = 0;
		_animState = ANIMSTATE::WALK;
		_modeState = ENEMYTYPE::DISCOVER;
	}
	return true;
};

bool Chainguard::ModeKnockBack() {
	return true;
};

bool Chainguard::SetGravity() {
	//重力処理
	if (_pos.y > 0) {
		_gravity++;
		_pos.y -= _gravity;
		if (_pos.y < 0) {
			_gravity = 0;
			_pos.y = 0;
		}
	}
	return true;
};

void Chainguard::SetKnockBackAndDamage(VECTOR vDir, float damage) {
	if (_knockBackSpeedFrame <= 0) {
		// プレイヤーの方向に向くのとダメージ
		_rotation.y = atan2(vDir.x, vDir.z);
		_hp -= damage;

		// 攻撃エフェクト発生
		VECTOR effectPos = VAdd(VAdd(_pos, _diffeToCenter), VScale(vDir, -50));
		int effectHandle[30];
		ResourceServer::LoadMultGraph("split", "res/TemporaryMaterials/split/test", ".png", 30, effectHandle);
		BoardPolygon* effect = new BoardPolygon(effectPos, GetCameraBillboardMatrix(), 200, effectHandle, 30, 0.5f / 60.0f * 1000.0f);
		EffectManeger::GetInstance()->LoadEffect(effect);

		// 状態を追跡所歌に変更
		_modeState = ENEMYTYPE::DISCOVER;

		// hpが0以下の場合
		if (_hp <= 0) {
			// 制圧ゲージを減らす
			Suppression::GetInstance()->SubSuppression(_suppression);
			// プレイヤーに経験値を付与
			_player->SetExp(_weightExp);
			// 状態をDEADに変更
			_modeState = ENEMYTYPE::DEAD;
		}
	}
};

bool Chainguard::IndividualProcessing(){
	_animManager->Process(static_cast<int>(_animState));
	_frameData->Process(static_cast<int>(_animState), _animManager->GetPlayTime());
	return true;
};

bool Chainguard::IndividualRendering() {
	return true;
};