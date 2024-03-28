#include "Player.h"
#include "MotionList.h"

Player* Player::_instance = NULL;
std::map<int, ANIMATION_INFO> Player::_animMap;

namespace {
	// 最大レベル
	constexpr int LEVEL_MAX = 9;
	// 最大HP
	constexpr int HP_MAX = 4;
	// 最大無敵時間
	constexpr int INVINCIBLE_CNT_MAX = 240;

	// 移動速度（通常状態）
	// 最大値
	constexpr float MOVE_SPEED_MAX = 14.0f;
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

	// 回転攻撃のカウント
	// このフレーム数以上の間、ボタンを押し続けると回転攻撃が発動する
	constexpr int ROTAION_SWING_CNT_MAX = 25;

	// スタミナの最大値
	constexpr float STAMINA_MAX = 480.0f;
	// 回転攻撃の1フレームあたりのスタミナ消費量
	constexpr float ROTAION_SWING_STAMINA_DECREASE = 1.0f;
	// 回避行動のスタミナ消費量
	constexpr float AVOIDANCE_STAMINA_DECREASE = STAMINA_MAX / 6.0f;
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
	constexpr unsigned int 	C_P_SET_INVINCIBLE_CNT						= 7;

	constexpr unsigned int 	C_P_ENABLE_IB_ATTACK_COLLISION		= 100;
	constexpr unsigned int 	C_P_ENABLE_IB_FOLLOWING_MODE		= 101;
	constexpr unsigned int 	C_P_ENABLE_IB_INTERPOLATION			= 102;
}

Player::Player()
{
	_input = nullptr;
	_inputWorldDir = VGet(0.0f, 0.0f, -1.0f);

	_hp = 0;
	_isInvincible = false;
	_invincibleRemainingCnt = 0;
	_idleFightingRemainingCnt = 0;

	_stamina = 0;
	_staminaMax = 0;
	_isRecoveringStamina = true;
	_cntToStartRecoveryStamina = 0;
	_isTired = false;
	_staminaRecoverySpeed = 0;

	_canMove = false;
	_moveSpeed = 0.0f;
	_moveSpeedFWD = 0.0f;

	_isAttackState = false;
	_isSwinging = false;
	_isRotationSwinging = false;
	_rotationCnt = 0;

	_canMotionCancel = false;
	_playNextComboAnim = false;

	_animManager = nullptr;
	_animStatus = ANIM_STATE::IDLE;
	_frameData = nullptr;

	_ironBall = nullptr;

	_capsuleCollision.r = 0.0f;
	_capsuleCollision.up = 0.0f;


	_blastOffDir = VGet(0, 0, 0);
	_blastOffPower = 0.0f;

	_rightHandFrameIndex = -1;

	global._oldExp = global.GetAllExp();


	_nowLevel = 0;
	_maxLevel = 0;
	global.SetOldExp(global.GetAllExp());
	_power = 0;

	_nextLevel.clear();
	_animManager = nullptr;
	_frameData = nullptr;
	_modelColor = nullptr;



	_instance = nullptr;
}

Player::~Player()
{
	_instance = nullptr;
	_input = nullptr;
	delete _animManager;
	delete _frameData;
	delete _modelColor;
	delete _ironBall;

	for (auto&& bone : _bone) {
		delete bone;
	}
	_bone.clear();

	global.SetAllExpAdd(_nowExp);
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
	// ダメージサウンドの再生
	switch (_hp) {
	case 0:
		global._soundServer->DirectPlay("PL_GameOver");
		break;
	case 1:
		global._soundServer->DirectPlay("PL_HealthAlert");
		break;
	case 2:
	case 3:
		int voiceNum = rand() % 2 + 1;
		std::string voiceName = "PL_Damage0" + std::to_string(voiceNum);
		global._soundServer->DirectPlay(voiceName);
		break;
	}
}

void Player::SetBone() {
	//左髪
	std::vector<int> Left_mitsuami(6);
	Left_mitsuami[0] = MV1SearchFrame(_modelHandle, "Left_mitsuami1");
	Left_mitsuami[1] = MV1SearchFrame(_modelHandle, "Left_mitsuami2");
	Left_mitsuami[2] = MV1SearchFrame(_modelHandle, "Left_mitsuami3");
	Left_mitsuami[3] = MV1SearchFrame(_modelHandle, "Left_mitsuami4");
	Left_mitsuami[4] = MV1SearchFrame(_modelHandle, "Left_mitsuami5");
	Left_mitsuami[5] = MV1SearchFrame(_modelHandle, "Left_mitsuami6");
	_bone.push_back(NEW bone(&_modelHandle, Left_mitsuami, Left_mitsuami.size() - 2, "Data/BoneParam/Hear.json"));
	//右髪
	std::vector<int> Right_mitsuami(6);
	Right_mitsuami[0] = MV1SearchFrame(_modelHandle, "Right_mitsuami1");
	Right_mitsuami[1] = MV1SearchFrame(_modelHandle, "Right_mitsuami2");
	Right_mitsuami[2] = MV1SearchFrame(_modelHandle, "Right_mitsuami3");
	Right_mitsuami[3] = MV1SearchFrame(_modelHandle, "Right_mitsuami4");
	Right_mitsuami[4] = MV1SearchFrame(_modelHandle, "Right_mitsuami5");
	Right_mitsuami[5] = MV1SearchFrame(_modelHandle, "Right_mitsuami6");
	_bone.push_back(NEW bone(&_modelHandle, Right_mitsuami, Right_mitsuami.size() - 2, "Data/BoneParam/Hear.json"));
	//スカーフ下部分
	std::vector<int> Scarf_Underf(3);
	Scarf_Underf[0] = MV1SearchFrame(_modelHandle, "Under_sukafu1");
	Scarf_Underf[1] = MV1SearchFrame(_modelHandle, "Under_sukafu2");
	Scarf_Underf[2] = MV1SearchFrame(_modelHandle, "Under_sukafu3");
	_bone.push_back(NEW bone(&_modelHandle, Scarf_Underf, Scarf_Underf.size() - 2, "Data/BoneParam/Scarf.json"));
	//リボン左下
	std::vector<int> Left_Sarope(3);
	Left_Sarope[0] = MV1SearchFrame(_modelHandle, "Left_sarope1");
	Left_Sarope[1] = MV1SearchFrame(_modelHandle, "Left_sarope2");
	Left_Sarope[2] = MV1SearchFrame(_modelHandle, "Left_sarope3");
	_bone.push_back(NEW bone(&_modelHandle, Left_Sarope, Left_Sarope.size() - 2, "Data/BoneParam/Sarope.json"));
	//リボン右下
	std::vector<int> Right_Sarope(3);
	Right_Sarope[0] = MV1SearchFrame(_modelHandle, "Right_sarope1");
	Right_Sarope[1] = MV1SearchFrame(_modelHandle, "Right_sarope2");
	Right_Sarope[2] = MV1SearchFrame(_modelHandle, "Right_sarope3");
	_bone.push_back(NEW bone(&_modelHandle, Right_Sarope, Right_Sarope.size() - 2, "Data/BoneParam/Sarope.json"));
	//リボン左上
	std::vector<int> Left_ribbon(4);
	Left_ribbon[0] = MV1SearchFrame(_modelHandle, "Left_ribbon1");
	Left_ribbon[1] = MV1SearchFrame(_modelHandle, "Left_ribbon2");
	Left_ribbon[2] = MV1SearchFrame(_modelHandle, "Left_ribbon3");
	Left_ribbon[3] = MV1SearchFrame(_modelHandle, "Left_ribbon4");
	_bone.push_back(NEW bone(&_modelHandle, Left_ribbon, Left_ribbon.size() - 2, "Data/BoneParam/Ribbon.json"));
	//スカートリボン右下
	std::vector<int> Right_ribbon(4);
	Right_ribbon[0] = MV1SearchFrame(_modelHandle, "Right_ribbon1");
	Right_ribbon[1] = MV1SearchFrame(_modelHandle, "Right_ribbon2");
	Right_ribbon[2] = MV1SearchFrame(_modelHandle, "Right_ribbon3");
	Right_ribbon[3] = MV1SearchFrame(_modelHandle, "Right_ribbon4");
	_bone.push_back(NEW bone(&_modelHandle, Right_ribbon, Right_ribbon.size() - 2, "Data/BoneParam/Ribbon.json"));
	//アホ毛
	std::vector<int> Ahoge(3);
	Ahoge[0] = MV1SearchFrame(_modelHandle, "Ahoge1");
	Ahoge[1] = MV1SearchFrame(_modelHandle, "Ahoge2");
	Ahoge[2] = MV1SearchFrame(_modelHandle, "Ahoge3");
	_bone.push_back(NEW bone(&_modelHandle, Ahoge, Ahoge.size() - 2, "Data/BoneParam/Ahoge.json"));
};

bool Player::HealHp(){
	if(_hp < HP_MAX){
		_hp++;
		global._soundServer->DirectPlay("SE_Heal");
		global._soundServer->DirectPlay("PL_Heal");
		return true;
	}
	return false;
};

bool Player::UpdateExp() {
	if (_nowLevel < _maxLevel) {
		if (_nowExp >= _nextLevel[_nowLevel]) {
			_nowExp -= _nextLevel[_nowLevel];
			global.SetAllExpAdd(_nextLevel[_nowLevel]);
			_nowLevel++;
			UpdateLevel();
		}
	}
	return true;
};

bool Player::Init(int modelHandle, VECTOR pos)
{
	CharacterBase::Init(modelHandle, pos);

	_input = XInput::GetInstance();
	_inputWorldDir = VGet(0.0f, 0.0f, -1.0f);

	_hp = HP_MAX;
	_isInvincible = false;
	_invincibleRemainingCnt = 0;

	_stamina = STAMINA_MAX;
	_staminaMax = STAMINA_MAX;
	_isRecoveringStamina = true;
	_isTired = false;
	_staminaRecoverySpeed = _staminaMax / STANIMA_RECOVERY_TIME;

	_canMove = true;
	_moveSpeed = 8.0f;
	_moveSpeedFWD = 0.0f;

	_isAttackState = false;
	_isSwinging = false;
	_isRotationSwinging = false;
	_rotationCnt = 0;

	_canMotionCancel = true;
	_playNextComboAnim = true;

	// モーションリストの読み込み
	MotionList::Load("Player", "MotionList_Player.csv");
	auto motionList = MotionList::GetMotionList("Player");
	// アニメーションマネージャーの初期設定
	_animManager = NEW AnimationManager();
	_animManager->InitMap("Player", _modelHandle, *motionList);
	// フレームデータの初期設定
	_frameData = NEW FrameData();
	_frameData->LoadData("Player", *motionList);
	// ステートを「待機」に設定
	_animStatus = ANIM_STATE::IDLE;


	// 鉄球の初期化
	_ironBall = NEW IronBall();
	_ironBall->Init();
	_ironBall->SetParentInstance(this);
	_ironBall->SetParentPosPtr(&_pos);
	_ironBall->SetPlayerModelHandle(_modelHandle);
	// 鉄球の移動状態を「追従」に設定
	_ironBall->SetMoveState(IB_MOVE_STATE::FOLLOWING);

	// 当たり判定の初期設定
	_capsuleCollision.r = 30.0f;
	_capsuleCollision.up = 65.0f;
	UpdateCollision();

	_cell->_objType = OBJ_TYPE::PL;

	_blastOffDir = VGet(0, 0, 0);
	_blastOffPower = 0.0f;

	_rightHandFrameIndex = MV1SearchFrame(_modelHandle, "Character1_RightHand");

	_modelColor = NEW ModelColor();
	_modelColor->Init(_modelHandle);

	SetBone();




	_instance = this;

	_idleFightingRemainingCnt = 0;

	_nowLevel = 0;

	SetLevelParam("res/JsonFile/IronState.json");
	UpdateLevel();












	return true;
}

bool Player::Process(float camAngleY)
{
	// フレームデータの実行コマンドをチェックする
	CheckFrameDataCommand();

	if(_hp <= 0){
		_animStatus = ANIM_STATE::GAMEOVER;
	}

	if (_animStatus != ANIM_STATE::GAMEOVER) {

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
					float speedMin = _isTired ? MOVE_SPEED_TIRED_MIN : MOVE_SPEED_MIN;

					_moveSpeed = Easing::Linear(rate, speedMin, speedMax, 1.0f);
					if (rate > MOVE_RUN_THRESHOLD) _isRunnning = true;

					// 入力方向ベクトルを正規化する
					vMoveDir = VScale(vMoveDir, 1.0f / size);
					// 入力方向ベクトルにカメラの向きの補正をかけて移動方向を決定する
					MATRIX mRot = MGetRotY(camAngleY);
					vMoveDir = VTransform(vMoveDir, mRot);

					_pos = VAdd(_pos, VScale(vMoveDir, _moveSpeed));

					_inputWorldDir = vMoveDir;
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
				else if (!_isAttackState) {
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
		if (_isRecoveringStamina) {
			_staminaRecoverySpeed = _staminaMax / STANIMA_RECOVERY_TIME;
			_stamina += _staminaRecoverySpeed;
			if (_stamina > _staminaMax) {
				_stamina = _staminaMax;
				_isTired = false;
				//_animStatus = ANIM_STATE::IDLE;
			}
		}

		if (_animStatus == ANIM_STATE::ROTATION_SWING) {
			_isRecoveringStamina = false;
			_stamina -= ROTAION_SWING_STAMINA_DECREASE;
			_cntToStartRecoveryStamina = 90;
			if (_stamina < 0.0f) {
				_stamina = 0.0f;
				_isTired = true;
				_isRotationSwinging = false;
				_rotationCnt = 0;
				_forwardDir = _inputWorldDir;
				_animStatus = ANIM_STATE::HORISONTAL_SWING_03;
			}
		}

		if (_cntToStartRecoveryStamina > 0) {
			_cntToStartRecoveryStamina -= 1;
			if (_cntToStartRecoveryStamina <= 0) {
				_isRecoveringStamina = true;
			}
		}

		// 攻撃状態の更新
		if (_isTired == false && _animStatus != ANIM_STATE::AVOIDANCE && _animStatus != ANIM_STATE::HIT) {
			// 回転攻撃が発生するかどうかの判定
			if (_rotationCnt > ROTAION_SWING_CNT_MAX) {
				if (!_isRotationSwinging) {
					_animStatus = ANIM_STATE::TO_ROTATION_SWING;
				}
			}
			// コンボ攻撃1段目の入力
			else if (!_isAttackState) {
				if (_input->GetRel(XINPUT_BUTTON_X) != 0) { // リリース入力
					_playNextComboAnim = true;
					if (!_isAttackState) {
						_animStatus = ANIM_STATE::HORISONTAL_SWING_01;
					}
				}
			}
			// コンボ攻撃2段目・3段目の入力
			else if (_animStatus == ANIM_STATE::HORISONTAL_SWING_01 || _animStatus == ANIM_STATE::HORISONTAL_SWING_02) {
				if (_input->GetTrg(XINPUT_BUTTON_X) != 0) { // トリガ入力
					_playNextComboAnim = true;
				}
			}

			// 回転攻撃の入力
			if (_input->GetKey(XINPUT_BUTTON_X) != 0) {
				_rotationCnt++;
			}
			else {
				_rotationCnt = 0;
				if (_isRotationSwinging) {
					_animStatus = ANIM_STATE::HORISONTAL_SWING_03;

					// モデルの正面方向を更新する
					_forwardDir = _inputWorldDir;
				}
			}
		}

		if (!_isRotationSwinging && _isAttackState) {
			_rotationCnt = 0;
		}

		if (!_isTired && _canMotionCancel) {
			// 回避
			if (_input->GetTrg(XINPUT_BUTTON_A)) {
				if (!_isSwinging || _isRotationSwinging) {
					_animStatus = ANIM_STATE::AVOIDANCE;
					// モデルの正面方向を更新する
					_forwardDir = _inputWorldDir;
					_rotationCnt = 0;
					_idleFightingRemainingCnt = 240;

					_isRecoveringStamina = false;
					_cntToStartRecoveryStamina = 90;
					_stamina -= AVOIDANCE_STAMINA_DECREASE;
					if (_stamina < 0.0f) {
						_stamina = 0.0f;
						_isTired = true;
					}
				}
			}
		}

		if (_isRotationSwinging) {
			float angle = _animStatus == ANIM_STATE::TO_ROTATION_SWING ? -(2.0f * DX_PI_F) / 80.0f : -(2.0f * DX_PI_F) / 30.0f;
			_forwardDir = VTransform(_forwardDir, MGetRotY(angle));
		}

		// 回転処理
		Math::SetModelForward_RotationY(_modelHandle, _forwardDir);

	}

	BlastOffProcess();

	UpdateCollision();
	//-------------------
	//齋藤が作成した関数です。
	UpdateExp();
	UpdateBone();
	//-------------------

	


	_ironBall->Process();

	_collisionManager->UpdateCell(_cell);

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
	_ironBall->Render();
	return true;
}



void Player::UpdateCollision()
{
	_capsuleCollision.down_pos = VAdd(_pos, VGet(0, _capsuleCollision.r, 0));
	_capsuleCollision.Update();
}

void Player::SetLevel(int allExp){
	int exp = allExp;
	while(1){
		if (_nowLevel >= LEVEL_MAX)break;// 最大レベル

		if(exp >= _nextLevel[_nowLevel]){
			exp -= _nextLevel[_nowLevel];
			_nowLevel++;
		}else{
			_nowExp = exp;
			break;
		}
	}
	_power = _levelParam[_nowLevel].power;
	_ironBall->UpdateLevel(_levelParam[_nowLevel].magnification);
	_staminaMax = _levelParam[_nowLevel].stamina;
};

void Player::SetLevelParam(std::string FileName)
{
	myJson json(FileName);
	_maxLevel = json._size - 1;
	int level = 0;
	int exp = 0;
	LevelData data;
	for (auto& list : json._json) {
		// レベルと経験値を取得
		list.at("Level").get_to(level);
		list.at("Exp").get_to(exp);
		_nextLevel[level] = exp;
		// 攻撃力と拡大率・スタミナを取得
		list.at("Power").get_to(data.power);
		list.at("Magnification").get_to(data.magnification);
		list.at("Stamina").get_to(data.stamina);
		_levelParam[level] = data;
	}
}

bool Player::UpdateLevel()
{
	_power = _levelParam[_nowLevel].power;
	_ironBall->UpdateLevel(_levelParam[_nowLevel].magnification);
	_staminaMax =  _levelParam[_nowLevel].stamina;
	if (_nowLevel > 0) {
		// レベルアップエフェクト
		float size = 5.0f * _levelParam[_nowLevel].magnification;
		VECTOR* pos = GetIBPosPtr();
		int effectHandle = ResourceServer::Load("FX_3D_Level_Up", "res/Effekseer/FX_3D_Level_Up/FX_3D_Level_Up.efkefc");
		EffekseerPosSynchro* effect = new EffekseerPosSynchro(effectHandle, pos, size);
		EffectManeger::GetInstance()->LoadEffect(effect);
		// レベルアップボイス
		int randomNum = rand() % 2 + 1; // ランダムで音声を再生　1~2
		std::string voiceName = "PL_LevelUp0" + std::to_string(randomNum);
		global._soundServer->DirectPlay(voiceName);
	}
	return true;
}

void Player::UpdateBone() {
	for (int i = 0; i < 2; i++) {
		// 髪の毛の重力を変更
		_bone[i]->SetGravity("Character1_Spine", "Character1_Head");
	}
	for (auto&& bone : _bone) {
		bone->Process();
		bone->SetMain(bone->_massPosList);
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
		case C_P_SET_INVINCIBLE_CNT:
			ChangeIsInvincible(true, static_cast<int>(param));
			break;

		case C_P_ENABLE_IB_ATTACK_COLLISION:
			_ironBall->SetEnabledAttackCollision(static_cast<bool>(param));
			break;
		case C_P_ENABLE_IB_FOLLOWING_MODE:
		{
			IB_MOVE_STATE nextState = static_cast<int>(param) == 0 ? IB_MOVE_STATE::PUTTING_ON_SOCKET : IB_MOVE_STATE::FOLLOWING;
			_ironBall->SetMoveState(nextState);
			break;
		}
		case C_P_ENABLE_IB_INTERPOLATION:
			_ironBall->SetMoveState(IB_MOVE_STATE::INTERPOLATION);
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
	int y = 500;
	int line = 0;
	//DrawFormatString(x, y + line * 16, COLOR_RED, "HP:%d", _hp); line++;
	//DrawFormatString(x, y + line * 16, COLOR_RED, "isInvincible:%d", _isInvincible); line++;
	//DrawFormatString(x, y + line * 16, COLOR_RED, "invincibleCnt:%d", _invincibleRemainingCnt); line++;
	DrawFormatString(x, y + line * 16, COLOR_RED, "_rotationCnt:%d", _rotationCnt); line++;

	//DrawFormatString(x, y + line * 16, COLOR_RED, "ANIM:%d", _animStatus); line++;
	//DrawCapsule3D(_capsuleCollision._startPos, _capsuleCollision._endPos, _capsuleCollision._r, 16, COLOR_RED, COLOR_RED, false);


	_animManager->DrawDebugInfo();
	_ironBall->DrawDebugInfo();
}
