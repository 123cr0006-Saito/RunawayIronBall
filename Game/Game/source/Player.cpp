#include "Player.h"
#include "appframe.h"

Player* Player::_instance = NULL;
std::map<int, ANIMATION_INFO> Player::_animMap;

namespace {
	// 最大HP
	constexpr int HP_MAX = 4;
	// 最大無敵時間
	constexpr int INVINCIBLE_CNT_MAX = 90;

	// 移動速度
	// 最大値
	constexpr float MOVE_SPEED_MAX = 8.0f;
	// 最小値
	constexpr float MOVE_SPEED_MIN = 1.0f;

	// スティック入力ベクトルの大きさの最大値
	// この値を最大値として、入力の大きさを割合を計算する（割合は0~1の範囲にクランプする）
	constexpr float MOVE_DIR_SIZE_MAX = 0.8f;

	// 「走り」状態と「歩き」状態を切り替える閾値
	// 「スティック入力ベクトルの大きさ」がこの値を超えたら「走り」状態になる
	constexpr float MOVE_RUN_THRESHOLD = 0.6f;
}

Player::Player(int modelHandle, VECTOR pos) : CharacterBase(modelHandle, pos)
{
	_input = XInput::GetInstance();
	_stickDir = VGet(0, 0, -1);

	_hp = HP_MAX;
	_isInvincible = false;
	_invincibleRemainingCnt = 0;

	_canMove = true;
	_moveSpeed = 8.0f;
	_moveSpeedFD = 0.0f;

	_capsuleCollision.r = 30.0f;
	_capsuleCollision.up = 65.0f;
	UpdateCollision();

	// 鉄球の移動状態を「追従」に設定
	_ibMoveState = IB_MOVE_STATE::FOLLOWING;

	_isAttackState = false;

	_playNextComboAnim = true;


	// ステートを「待機」に設定
	_animStatus = ANIM_STATE::IDLE;


	_blastOffDir = VGet(0, 0, 0);
	_blastOffPower = 0.0f;

	_rightHandFrameIndex = MV1SearchFrame(_modelHandle, "Character1_RightHand");

	SetBone();

	_isSwinging = false;
	_isRotationSwinging = false;
	_spinCnt = 0;


	_instance = this;

	_idleFightingRemainingCnt = 0;
	// アニメーションマネージャーの初期設定
	_animManager = new AnimationManager();
	_animManager->InitMap("Player", _modelHandle, "MotionList_Player.csv");


	_frameData = new FrameData();
	std::vector<std::pair<int, std::string>> fdFileName;
	fdFileName.push_back(std::make_pair(static_cast<int>(ANIM_STATE::HORISONTAL_SWING_01), "FD_MO_PL_Horizontal_first.csv"));
	fdFileName.push_back(std::make_pair(static_cast<int>(ANIM_STATE::HORISONTAL_SWING_02), "FD_MO_PL_Horizontal_second.csv"));
	fdFileName.push_back(std::make_pair(static_cast<int>(ANIM_STATE::HORISONTAL_SWING_03), "FD_MO_PL_Horizontal_third.csv"));
	fdFileName.push_back(std::make_pair(static_cast<int>(ANIM_STATE::IDLE), "FD_MO_PL_Idle.csv"));
	fdFileName.push_back(std::make_pair(static_cast<int>(ANIM_STATE::IDLE_FIGHTING), "FD_MO_PL_Idle_Fighting.csv"));
	fdFileName.push_back(std::make_pair(static_cast<int>(ANIM_STATE::ROTATION_SWING), "FD_MO_PL_Rotate_Loop.csv"));
	fdFileName.push_back(std::make_pair(static_cast<int>(ANIM_STATE::AVOIDANCE), "FD_MO_PL_Avoidance.csv"));
	_frameData->LoadData("Player", fdFileName);

	//_animManager->InitMap(&_animMap);

	//// _animMapが空の場合、アニメーション情報を設定する
	//// _animMapは静的メンバ変数なので、インスタンス間で共有する
	//if (_animMap.empty() ){
	//	// _animMapの初期設定
	//	_animManager->SetupAnimationInfo(static_cast<int>(STATUS::WAIT), MV1GetAnimIndex(_modelHandle, "MO_PL_Stay"), 0);
	//	_animManager->SetupAnimationInfo(static_cast<int>(STATUS::RUN), MV1GetAnimIndex(_modelHandle, "MO_PL_Run"), 0);
	//	_animManager->SetupAnimationInfo(static_cast<int>(STATUS::HORISONTAL_SWING_01), MV1GetAnimIndex(_modelHandle, "MO_PL_Horizontal_first"), 1);
	//	_animManager->SetupAnimationInfo(static_cast<int>(STATUS::HORISONTAL_SWING_02), MV1GetAnimIndex(_modelHandle, "MO_PL_Horizontal_second"), 1);
	//	_animManager->SetupAnimationInfo(static_cast<int>(STATUS::SPIN_SWING), MV1GetAnimIndex(_modelHandle, "MO_PL_roteate_loop"), 1);
	//}

	_modelColor = new ModelColor();
	_modelColor->Init(_modelHandle);
}

Player::~Player()
{
	_instance = nullptr;
	delete _animManager;
}

// 無敵状態の更新
void Player::ChangeIsInvincible(bool b)
{
	_isInvincible = b;
	if (b) {
		_invincibleRemainingCnt = INVINCIBLE_CNT_MAX;
	}
	else {
		_invincibleRemainingCnt = 0;
	}
}

void Player::SetBone() {
	//左髪
	std::vector<int> bone_left_list(6);
	bone_left_list[0] = MV1SearchFrame(_modelHandle,"Left_mitsuami1");
	bone_left_list[1] = MV1SearchFrame(_modelHandle,"Left_mitsuami2");
	bone_left_list[2] = MV1SearchFrame(_modelHandle,"Left_mitsuami3");
	bone_left_list[3] = MV1SearchFrame(_modelHandle,"Left_mitsuami4");
	bone_left_list[4] = MV1SearchFrame(_modelHandle,"Left_mitsuami5");
	bone_left_list[5] = MV1SearchFrame(_modelHandle,"Left_mitsuami6");
	_bone[0] = new bone(&_modelHandle, bone_left_list, bone_left_list.size() - 2, "res/JsonFile/hair_parameters.json");
	//右髪
	std::vector<int> bone_right_list(6);
	bone_right_list[0] = MV1SearchFrame(_modelHandle,"Right_mitsuami1");
	bone_right_list[1] = MV1SearchFrame(_modelHandle,"Right_mitsuami2");
	bone_right_list[2] = MV1SearchFrame(_modelHandle,"Right_mitsuami3");
	bone_right_list[3] = MV1SearchFrame(_modelHandle,"Right_mitsuami4");
	bone_right_list[4] = MV1SearchFrame(_modelHandle,"Right_mitsuami5");
	bone_right_list[5] = MV1SearchFrame(_modelHandle,"Right_mitsuami6");
	_bone[1] = new bone(&_modelHandle, bone_right_list, bone_right_list.size() - 2, "res/JsonFile/hair_parameters.json");
};

void Player::SetNextExp(std::string FileName) {
	myJson json(FileName);
	_maxLevel = json._size - 1;
	for(auto& expList : json._json) {
		int nowLevel = 0;
		int exp = 0;
		expList.at("Level").get_to(nowLevel);
		expList.at("Exp").get_to(exp);
		_nextLevel[nowLevel] = exp;
	}
};

bool Player::UpdateExp() {
	if (_nowLevel < _maxLevel) {
		if (_nowExp >= _nextLevel[_nowLevel]) {
			_nowExp -= _nextLevel[_nowLevel];
			_nowLevel++;
		}
	}

	if (_input->GetTrg(XINPUT_BUTTON_A)) {
		if (_nowLevel <= _maxLevel) {
			_nowLevel--;
		}
	}
	if (_input->GetTrg(XINPUT_BUTTON_B)) {
		if (_nowLevel < _maxLevel) {
			_nowLevel++;
		}
	}
	return true;
};

bool Player::Process(float camAngleY)
{
	// フレームデータの実行コマンドをチェックする
	CheckFrameDataCommand();


	// 無敵状態関連の処理
	if (_isInvincible) {
		int cnt = 10;

		bool b = (INVINCIBLE_CNT_MAX - _invincibleRemainingCnt) % (cnt * 2) < cnt;
		_modelColor->ChangeFlickerTexture(b);

		// 無敵時間を減らす
		_invincibleRemainingCnt -= 1;
		// 無敵時間が0以下になったら無敵状態を解除する
		if (_invincibleRemainingCnt < 0) {
			_isInvincible = false;
			_modelColor->ChangeFlickerTexture(false);
		}
	}

	{
		// 移動処理

		// 移動フラグ
		bool _isMoved = false;

		bool _isRunnning = false;

		// 左スティックの入力情報を取得する
		auto lStick = _input->GetAdjustedStick_L();
		VECTOR vMoveDir = VGet(lStick.x, 0, lStick.y);
		if (_canMove) {
			// 左スティックの入力があったら
			if (VSize(vMoveDir) > 0.000000f) {

				float size = VSize(vMoveDir);

				float rate = size / MOVE_DIR_SIZE_MAX;
				_moveSpeed = MOVE_SPEED_MAX * rate;
				rate = Math::Clamp(0.0f, 1.0f, rate);
				_moveSpeed = Easing::Linear(rate, MOVE_SPEED_MIN, MOVE_SPEED_MAX, 1.0f);
				if(rate > MOVE_RUN_THRESHOLD) _isRunnning = true;

				// 入力方向ベクトルを正規化する
				vMoveDir = VScale(vMoveDir, 1.0f / size);
				// 入力方向ベクトルにカメラの向きの補正をかけて移動方向を決定する
				MATRIX mRot = MGetRotY(camAngleY);
				vMoveDir = VTransform(vMoveDir, mRot);

				_pos = VAdd(_pos, VScale(vMoveDir, _moveSpeed));

				_stickDir = vMoveDir;
				_isMoved = true;
			}
		}

		if (_moveSpeedFD != 0.f) {
			_pos = VAdd(_pos, VScale(VNorm(_forwardDir), _moveSpeedFD));
		}

		if (!_isAttackState && _animStatus != ANIM_STATE::AVOIDANCE) {
			if (_isMoved) {
				if (_isRunnning) {
					_animStatus = ANIM_STATE::RUN;
				}
				else {
					_animStatus = ANIM_STATE::WALK;
				}

				// キャラクターを滑らかに回転させる
				float angle = Math::CalcVectorAngle(_forwardDir, vMoveDir);
				float rotRad = (2.0f * DX_PI_F) / 30.0f;
				if (rotRad > angle) {
					_forwardDir = vMoveDir;
				}
				else {
					VECTOR vN = VCross(_forwardDir, vMoveDir);
					_forwardDir = VTransform(_forwardDir, MGetRotAxis(vN, rotRad));
				}
			}
			else {
				if (_idleFightingRemainingCnt > 0) {
					_animStatus = ANIM_STATE::IDLE_FIGHTING;
					_idleFightingRemainingCnt -= 1;
				}
				else {
					_animStatus = ANIM_STATE::IDLE_FIGHTING;
				}
			}
		}
	}

	if (_animStatus != ANIM_STATE::AVOIDANCE) {
		// 回転攻撃
		if (_spinCnt > 90) {
			_animStatus = ANIM_STATE::ROTATION_SWING;
		}
		// 通常攻撃
		else if (_input->GetRel(XINPUT_BUTTON_X) != 0) {
			_playNextComboAnim = true;
			if (!_isAttackState) {
				_animStatus = ANIM_STATE::HORISONTAL_SWING_01;
			}
		}

		if (_input->GetKey(XINPUT_BUTTON_X) != 0) {
			_spinCnt++;
		}
		else {
			_spinCnt = 0;
			if (_isRotationSwinging) {
				_animStatus = ANIM_STATE::HORISONTAL_SWING_03;

				// モデルの正面方向を更新する
				_forwardDir = _stickDir;
			}
		}

		// 回避
		if (_input->GetTrg(XINPUT_BUTTON_A)) {
			if (!_isSwinging || _isRotationSwinging) {
				_animStatus = ANIM_STATE::AVOIDANCE;
				// モデルの正面方向を更新する
				_forwardDir = _stickDir;
				_spinCnt = 0;
			}
		}
	}


	if (_isRotationSwinging) {
		_forwardDir = VTransform(_forwardDir, MGetRotY(-(2.0f * DX_PI_F) / 30.0f));
	}

	// 回転処理
	Math::SetModelForward_RotationY(_modelHandle, _forwardDir);

	BlastOffProcess();

	MV1SetPosition(_modelHandle, _pos);
	UpdateCollision();
	//-------------------
	//齋藤が作成した関数です。
	UpdateExp();
	UpdateBone();
	//-------------------

	_animManager->Process(static_cast<int>(_animStatus));
	_frameData->Process(static_cast<int>(_animStatus), _animManager->GetPlayTime());





	return true;
}


bool Player::BlastOffProcess()
{
	if (_blastOffPower > 0.0f) {
		VECTOR tmp = VNorm(_blastOffDir);
		if (VSize(tmp) > 0.0f) {
			_pos = VAdd(_pos, VScale(tmp, _blastOffPower));
		}
		_blastOffPower -= 1.0f;
	}
	return true;
}

bool Player::Render()
{
	CharacterBase::Render();
	return true;
}



void Player::UpdateCollision()
{
	_capsuleCollision.down_pos = VAdd(_pos, VGet(0, _capsuleCollision.r, 0));
	_capsuleCollision.Update();
}

void Player::UpdateBone() {
	for (int i = 0; i < sizeof(_bone) / sizeof(_bone[0]); i++) {
		_bone[i]->Process();
		_bone[i]->SetMain(_bone[i]->_massPosList);
	}
	if (_input->GetTrg(XINPUT_BUTTON_DPAD_DOWN)) {
		for (int i = 0; i < sizeof(_bone) / sizeof(_bone[0]); i++) {
			_bone[i]->SetDebugDraw();
			_bone[i]->DebugProcess(12);
		}
	}
};

VECTOR Player::GetRightHandPos()
{
	VECTOR handPos = VGet(0.0f, 0.0f, 0.0f);
	MATRIX m = MV1GetFrameLocalWorldMatrix(_modelHandle, _rightHandFrameIndex);
	handPos = VTransform(handPos, m);
	return handPos;
}

void Player::CheckFrameDataCommand()
{
	// 実行コマンドリストを取得する
	std::vector<CommandParam> commandList = _frameData->GetCommandData();

	for (auto itr = commandList.begin(); itr != commandList.end(); ++itr) {
		// コマンドを取得する
		int command = itr->first;
		// パラメータを取得する
		float param = itr->second;

		// コマンドによって処理を分岐する
		switch (command)
		{
		// モーションを変更する
		case C_P_CHANGE_MOTION:
			_animStatus = static_cast<ANIM_STATE>(param);
			break;
		// 移動可能状態を変更する
		case C_P_ENABLE_MOVE:
			_canMove = static_cast<bool>(param);
			break;
		case C_P_MOVE_FORWARD:
			_moveSpeedFD = param;
			break;
		// コンボの入力受付を開始する
		// C_P_CHECK_CHANGE_COMBOが実行されるタイミングで_playNextComboAnimがtrueの場合に次のコンボモーションを再生する
		case C_P_ACCEPT_COMBO_INPUT:
			_playNextComboAnim = false;
			break;
		// コンボモーションの変更をチェックする
		case C_P_CHECK_CHANGE_COMBO:
			if (_playNextComboAnim) {
				switch (_animStatus)
				{
				case Player::ANIM_STATE::HORISONTAL_SWING_01:
					_animStatus = ANIM_STATE::HORISONTAL_SWING_02;
					break;
				case Player::ANIM_STATE::HORISONTAL_SWING_02:
					_animStatus = ANIM_STATE::HORISONTAL_SWING_03;
					break;
				}
			}
			break;
		// 攻撃状態の変更をチェックする	
		case C_P_CHECK_CHANGE_ATTACK_STATE:
		{
			// 次の攻撃状態を取得する
			bool nextState = static_cast<bool>(param);
			// 次の攻撃状態がfalseの場合、攻撃状態を解除する
			if (!nextState){
				_isSwinging = false;
				_isRotationSwinging = false;
				// 前の攻撃状態がtrueの場合、戦闘待機状態に遷移する
				if (_isAttackState) {
					_idleFightingRemainingCnt = 240;
				}
			}
			else {
				// 回転攻撃の場合、回転攻撃フラグを立てる
				if (_animStatus == ANIM_STATE::ROTATION_SWING) {
					_isRotationSwinging = true;
					_isSwinging = false;
				}
				// 通常攻撃の場合、通常攻撃フラグを立てる
				else {
					_isRotationSwinging = false;
					_isSwinging = true;
				}
			}
			// 攻撃状態を更新する
			_isAttackState = nextState;
			break;
		}
		case C_P_ENABLE_IB_ATTACK_COLLISION:
			break;
		case C_P_ENABLE_IB_FOLLOWING_MODE:
			_ibMoveState = static_cast<int>(param) == 0 ? IB_MOVE_STATE::PUTTING_ON_SOCKET : IB_MOVE_STATE::FOLLOWING;
			break;
		case C_P_ENABLE_IB_INTERPOLATION:
			_ibMoveState = IB_MOVE_STATE::INTERPOLATION;
			break;
		}
	}
}

void Player::DrawDebugInfo()
{
	for (int i = 0; i < sizeof(_bone) / sizeof(_bone[0]); i++) {
		_bone[i]->DebugRender();
	}

	DrawCapsule3D(_capsuleCollision.up_pos, _capsuleCollision.down_pos, _capsuleCollision.r, 16, COLOR_RED, COLOR_RED, false);

	int x = 0;
	int y = 100;
	int line = 0;
	DrawFormatString(x, y + line * 16, COLOR_RED, "HP:%d", _hp); line++;
	DrawFormatString(x, y + line * 16, COLOR_RED, "isInvincible:%d", _isInvincible); line++;
	DrawFormatString(x, y + line * 16, COLOR_RED, "invincibleCnt:%d", _invincibleRemainingCnt); line++;
	//DrawCapsule3D(_capsuleCollision._startPos, _capsuleCollision._endPos, _capsuleCollision._r, 16, COLOR_RED, COLOR_RED, false);
}
