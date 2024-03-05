#include "Player.h"
#include "MotionList.h"

Player* Player::_instance = NULL;
std::map<int, ANIMATION_INFO> Player::_animMap;

namespace {
	// 最大HP
	constexpr int HP_MAX = 4;
	// 最大無敵時間
	constexpr int INVINCIBLE_CNT_MAX = 90;

	// 移動速度（通常状態）
	// 最大値
	constexpr float MOVE_SPEED_MAX = 8.0f;
	// 最小値
	constexpr float MOVE_SPEED_MIN = 1.0f;
	
	// 移動速度（疲れ状態）
	// 最大値
	constexpr float MOVE_SPEED_TIRED_MAX = 3.0f;
	// 最小値
	constexpr float MOVE_SPEED_TIRED_MIN = 0.5f;

	// スティック入力ベクトルの大きさの最大値
	// この値を最大値として、入力の大きさを割合を計算する（割合は0~1の範囲にクランプする）
	constexpr float MOVE_INPUT_VALUE_MAX = 0.8f;

	// 「走り」状態と「歩き」状態を切り替える閾値
	// 「スティック入力ベクトルの大きさ」がこの値を超えたら「走り」状態になる
	constexpr float MOVE_RUN_THRESHOLD = 0.6f;


	// スタミナの最大値
	constexpr float STAMINA_MAX = 480.0f;
	// 回転攻撃の1フレームあたりのスタミナ消費量
	constexpr float ROTAION_SWING_STAMINA_DECREASE = 1.0f;
	// スタミナが0になってから最大値まで回復するのにかかる時間
	constexpr float STANIMA_RECOVERY_TIME = 120.0f;

	// フレームデータのコマンド
	constexpr unsigned int	C_P_CHANGE_MOTION							= 0;
	constexpr unsigned int 	C_P_ENABLE_MOVE								= 1;
	constexpr unsigned int 	C_P_MOVE_FORWARD							= 2;
	constexpr unsigned int 	C_P_ACCEPT_COMBO_INPUT					= 3;
	constexpr unsigned int 	C_P_CHECK_CHANGE_COMBO				= 4;
	constexpr unsigned int 	C_P_CHECK_CHANGE_ATTACK_STATE		= 5;
	constexpr unsigned int 	C_P_ENACLE_MOTION_CANCEL				= 6;

	constexpr unsigned int 	C_P_ENABLE_IB_ATTACK_COLLISION		= 100;
	constexpr unsigned int 	C_P_ENABLE_IB_FOLLOWING_MODE		= 101;
	constexpr unsigned int 	C_P_ENABLE_IB_INTERPOLATION			= 102;
}

Player::Player(int modelHandle, VECTOR pos) : CharacterBase(modelHandle, pos)
{
	_input = XInput::GetInstance();
	_stickDir = VGet(0, 0, -1);

	_hp = HP_MAX;
	_isInvincible = false;
	_invincibleRemainingCnt = 0;

	_stamina = STAMINA_MAX;
	_staminaMax = STAMINA_MAX;
	_isConsumingStamina = false;
	_isTired = false;
	_staminaRecoverySpeed = _staminaMax / STANIMA_RECOVERY_TIME;

	_canMove = true;
	_moveSpeed = 8.0f;
	_moveSpeedFWD = 0.0f;

	_capsuleCollision.r = 30.0f;
	_capsuleCollision.up = 65.0f;
	UpdateCollision();



	_isAttackState = false;

	_canMotionCancel = true;
	_playNextComboAnim = true;

	// 鉄球の初期化
	_chain = new Chain();
	_chain->Init();
	_chain->SetPlayerModelHandle(_modelHandle);
	// 鉄球の移動状態を「追従」に設定
	_chain->SetMoveState(IB_MOVE_STATE::FOLLOWING);

	// ステートを「待機」に設定
	_animStatus = ANIM_STATE::IDLE;


	_blastOffDir = VGet(0, 0, 0);
	_blastOffPower = 0.0f;

	_rightHandFrameIndex = MV1SearchFrame(_modelHandle, "Character1_RightHand");

	SetBone();

	_isSwinging = false;
	_isRotationSwinging = false;
	_rotationCnt = 0;


	_instance = this;

	_idleFightingRemainingCnt = 0;


	_nowLevel = 0;
	SetNextExp("res/JsonFile/ExpList.json");
	SetPowerScale("res/JsonFile/IronState.json");
	UpdateLevel();

	// モーションリストの読み込み
	MotionList::Load("Player", "MotionList_Player.csv");
	auto motionList = MotionList::GetMotionList("Player");

	// アニメーションマネージャーの初期設定
	_animManager = new AnimationManager();
	_animManager->InitMap("Player", _modelHandle, *motionList);

	// フレームデータの初期設定
	_frameData = new FrameData();
	_frameData->LoadData("Player", *motionList);

	_modelColor = new ModelColor();
	_modelColor->Init(_modelHandle);
}

Player::~Player()
{
	_instance = nullptr;
	delete _animManager;
}

// 無敵状態の更新
void Player::ChangeIsInvincible(bool b, int frame)
{
	if (b) {
		if (!_isInvincible) {
			_invincibleRemainingCnt = frame;
		}
	}
	else {
		_invincibleRemainingCnt = 0;
	}
	_isInvincible = b;
}

void Player::SetDamage()
{
	_hp -= 1;
	if (_hp < 0) {
		_hp = 0;
	}
	_isSwinging = false;
	_isRotationSwinging = false;
	_rotationCnt = 0;
	_animStatus = ANIM_STATE::HIT;
	ChangeIsInvincible(true, INVINCIBLE_CNT_MAX);
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
			UpdateLevel();
		}
	}

	//if (_input->GetTrg(XINPUT_BUTTON_A)) {
	//	if (_nowLevel <= _maxLevel) {
	//		_nowLevel--;
	//		UpdateLevel();
	//	}
	//}
	//if (_input->GetTrg(XINPUT_BUTTON_B)) {
	//	if (_nowLevel < _maxLevel) {
	//		_nowLevel++;
	//		UpdateLevel();
	//	}
	//}
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
		if (_canMove || _canMotionCancel) {
			float size = VSize(vMoveDir);
			// 左スティックの入力があったら
			if (size > 0.000000f) {

				float rate = size / MOVE_INPUT_VALUE_MAX;
				rate = Math::Clamp(0.0f, 1.0f, rate);

				float speedMax = _isTired ? MOVE_SPEED_TIRED_MAX : MOVE_SPEED_MAX;
				float speedMin  = _isTired ? MOVE_SPEED_TIRED_MIN : MOVE_SPEED_MIN;

				_moveSpeed = Easing::Linear(rate, speedMin, speedMax, 1.0f);
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

		if (_moveSpeedFWD != 0.f) {
			_pos = VAdd(_pos, VScale(VNorm(_forwardDir), _moveSpeedFWD));
			_moveSpeedFWD = 0.f;
		}

		if (_canMotionCancel) {
			if (_isMoved) {
				if (_isTired) {
					_animStatus = ANIM_STATE::WALK_TIRED;
				}
				else {
					if (_isRunnning) {
						_animStatus = ANIM_STATE::RUN;
					}
					else {
						_animStatus = ANIM_STATE::WALK;
					}
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
			else if(!_isAttackState) {
				if (_isTired) {
					_animStatus = ANIM_STATE::IDLE_TIRED;
					_idleFightingRemainingCnt = 0;
				}
				else {
					if (_idleFightingRemainingCnt > 0) {
						_animStatus = ANIM_STATE::IDLE_FIGHTING;
						_idleFightingRemainingCnt -= 1;
					}
					else {
						_animStatus = ANIM_STATE::IDLE;
					}
				}
			}
		}
	}

	// スタミナの更新
	if (!_isConsumingStamina) {
		_staminaRecoverySpeed = STAMINA_MAX / STANIMA_RECOVERY_TIME;
		_stamina += _staminaRecoverySpeed;
		if (_stamina > STAMINA_MAX) {
			_stamina = STAMINA_MAX;
			_isTired = false;
			//_animStatus = ANIM_STATE::IDLE;
		}
	}

	if (_animStatus == ANIM_STATE::ROTATION_SWING) {
		_stamina -= ROTAION_SWING_STAMINA_DECREASE;
		_isConsumingStamina = true;
		if (_stamina < 0.0f) {
			_stamina = 0.0f;
			_isTired = true;
			_isRotationSwinging = false;
			_rotationCnt = 0;
			_forwardDir = _stickDir;
			_animStatus = ANIM_STATE::HORISONTAL_SWING_03;
		}
	}

	if(!_isAttackState ){
		_isConsumingStamina = false;
	}

	// 攻撃状態の更新
	if (_isTired == false && _animStatus != ANIM_STATE::AVOIDANCE && _animStatus != ANIM_STATE::HIT) {
		// 回転攻撃
		if (_rotationCnt > 90) {
			if (!_isRotationSwinging) {
				_animStatus = ANIM_STATE::TO_ROTATION_SWING;
			}
		}
		// 通常攻撃
		else if (_input->GetRel(XINPUT_BUTTON_X) != 0) {
			_playNextComboAnim = true;
			if (!_isAttackState) {
				_animStatus = ANIM_STATE::HORISONTAL_SWING_01;
			}
		}

		if (_input->GetKey(XINPUT_BUTTON_X) != 0) {
			_rotationCnt++;
		}
		else {
			_rotationCnt = 0;
			if (_isRotationSwinging) {
				_animStatus = ANIM_STATE::HORISONTAL_SWING_03;

				// モデルの正面方向を更新する
				_forwardDir = _stickDir;
			}
		}
	}

	if (_canMotionCancel) {
		// 回避
		if (_input->GetTrg(XINPUT_BUTTON_A)) {
			if (!_isSwinging || _isRotationSwinging) {
				_animStatus = ANIM_STATE::AVOIDANCE;
				// モデルの正面方向を更新する
				_forwardDir = _stickDir;
				_rotationCnt = 0;
				_idleFightingRemainingCnt = 240;
			}
		}
	}

	if (_isRotationSwinging) {
		float angle = _animStatus == ANIM_STATE::TO_ROTATION_SWING ? -(2.0f * DX_PI_F) / 80.0f : -(2.0f * DX_PI_F) / 30.0f;
		_forwardDir = VTransform(_forwardDir, MGetRotY(angle));
	}

	// 回転処理
	Math::SetModelForward_RotationY(_modelHandle, _forwardDir);

	BlastOffProcess();

	MV1SetPosition(_modelHandle, _pos);
	UpdateCollision();
	//-------------------
	//齋藤が作成した関数です。
	UpdateExp();
	//UpdateBone();
	//-------------------

	


	_chain->Process();



	return true;
}

bool Player::AnimationProcess()
{
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
	_chain->Render();
	return true;
}



void Player::UpdateCollision()
{
	_capsuleCollision.down_pos = VAdd(_pos, VGet(0, _capsuleCollision.r, 0));
	_capsuleCollision.Update();
}

void Player::SetPowerScale(std::string FileName)
{
	myJson json(FileName);
	int level = 0;
	int power = 0;
	float scale = 0;
	for (auto& list : json._json) {
		list.at("Level").get_to(level);
		list.at("Power").get_to(power);
		list.at("Magnification").get_to(scale);
		_powerAndScale[level] = std::make_pair(power, scale);
	}
}

bool Player::UpdateLevel()
{
	_power = _powerAndScale[_nowLevel].first;
	_chain->UpdateLevel(_powerAndScale[_nowLevel].second);
	return true;
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
			_moveSpeedFWD = param;
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
				if (_animStatus == ANIM_STATE::TO_ROTATION_SWING || _animStatus == ANIM_STATE::ROTATION_SWING) {
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
		case C_P_ENACLE_MOTION_CANCEL:
			_canMotionCancel = static_cast<bool>(param);
			break;

		case C_P_ENABLE_IB_ATTACK_COLLISION:
			_chain->SetEnabledAttackCollision(static_cast<bool>(param));
			break;
		case C_P_ENABLE_IB_FOLLOWING_MODE:
		{
			IB_MOVE_STATE nextState = static_cast<int>(param) == 0 ? IB_MOVE_STATE::PUTTING_ON_SOCKET : IB_MOVE_STATE::FOLLOWING;
			_chain->SetMoveState(nextState);
			break;
		}
		case C_P_ENABLE_IB_INTERPOLATION:
			_chain->SetMoveState(IB_MOVE_STATE::INTERPOLATION);
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

	//int x = 0;
	//int y = 100;
	//int line = 0;
	//DrawFormatString(x, y + line * 16, COLOR_RED, "HP:%d", _hp); line++;
	//DrawFormatString(x, y + line * 16, COLOR_RED, "isInvincible:%d", _isInvincible); line++;
	//DrawFormatString(x, y + line * 16, COLOR_RED, "invincibleCnt:%d", _invincibleRemainingCnt); line++;

	//DrawFormatString(x, y + line * 16, COLOR_RED, "ANIM:%d", _animStatus); line++;
	//DrawCapsule3D(_capsuleCollision._startPos, _capsuleCollision._endPos, _capsuleCollision._r, 16, COLOR_RED, COLOR_RED, false);


	_animManager->DrawDebugInfo();
	_chain->DrawDebugInfo();
}
