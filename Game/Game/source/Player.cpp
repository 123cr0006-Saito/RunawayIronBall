#include "Player.h"
#include "appframe.h"

Player* Player::_instance = NULL;

Player::Player(int modelHandle, VECTOR pos) : CharacterBase(modelHandle, pos)
{
	_input = XInput::GetInstance();

	_canMove = true;
	_speed = 8.0f;
	UpdateCollision();


	// アニメーションアタッチはされていない
	_attach_index = -1;
	// ステータスを「無し」に設定
	_animStatus = STATUS::NONE;
	// 再生時間の初期化
	_total_time = 0.f;
	_play_time = 0.0f;


	_blastOffDir = VGet(0, 0, 0);
	_blastOffPower = 0.0f;

	_rightHandFrameIndex = MV1SearchFrame(_modelHandle, "Character1_RightHand");

	SetBone();

	_isSwinging = false;
	_instance = this;
}

Player::~Player()
{
	if (_modelHandle != -1) {
		int n = 0;
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
	// 処理前のステータスを保存しておく
	STATUS oldStatus = _animStatus;
	//_animStatus = STATUS::NONE;


	if (_input->GetTrg(XINPUT_BUTTON_X) != 0) {
		_animStatus = STATUS::HORISONTAL_SWING;
		_isSwinging = true;
		_canMove = false;
	}

	if (!_isSwinging) {
		_animStatus = STATUS::WAIT;
		_canMove = true;
	}

	if (_canMove) {

		// 左スティックの入力情報を取得する
		auto lStick = _input->GetAdjustedStick_L();
		//auto rStick = _input->GetAdjustedStick_R();
		VECTOR vDir = VGet(lStick.x, 0, lStick.y);
		// 左スティックの入力があったら
		if (VSize(vDir) > 0.000000f) {
			// 移動処理
			vDir = VNorm(vDir);

			MATRIX mRot = MGetRotY(camAngleY);
			// 移動方向ベクトルを回転させる
			vDir = VTransform(vDir, mRot);
			_pos = VAdd(_pos, VScale(vDir, _speed));
			_animStatus = STATUS::RUN;

			// 回転処理
			// 基準のベクトル
			VECTOR vBase = VGet(0.0f, 0.0f, -1.0f);
			// 基準のベクトルと移動方向のベクトルのなす角を計算する
			float angle = Math::CalcVectorAngle(vDir, vBase);
			// 反時計回りの場合
			if (vDir.x > 0.0f) {
				angle *= -1;
			}
			// モデルの回転値をセットする
			MV1SetRotationXYZ(_modelHandle, VGet(0.0f, angle, 0.0f));
		}
		else {
			_animStatus = STATUS::WAIT;
		}


	}

	BlastOffProcess();

	MV1SetPosition(_modelHandle, _pos);
	UpdateCollision();
	//-------------------
	//齋藤が作成した関数です。
	UpdateExp();
	UpdateBone();
	//-------------------
	AnimProcess(oldStatus);
	return true;
}

bool Player::AnimProcess(STATUS oldStatus)
{
	if (oldStatus == _animStatus) {
		// 再生時間を進める
		_play_time += 1.0f;
	}
	else {
		// アニメーションがアタッチされていたら、デタッチする
		if (_attach_index != -1) {
			MV1DetachAnim(_modelHandle, _attach_index);
			_attach_index = -1;
		}
		// ステータスに合わせてアニメーションのアタッチ
		switch (_animStatus) {
		case STATUS::WAIT:
			_attach_index = MV1AttachAnim(_modelHandle, 0, -1, FALSE);
			break;
		case STATUS::HORISONTAL_SWING:
			_attach_index = MV1AttachAnim(_modelHandle, 2, -1, FALSE);
			break;
		case STATUS::RUN:
			_attach_index = MV1AttachAnim(_modelHandle, 1, -1, FALSE);
			break;
		}
		// アタッチしたアニメーションの総再生時間を取得する
		_total_time = MV1GetAttachAnimTotalTime(_modelHandle, _attach_index);
		// 再生時間を初期化
		_play_time = 0.0f;
	}

	// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
	if (_play_time >= _total_time) {
		_play_time = 0.0f;

		if (_animStatus == STATUS::HORISONTAL_SWING) {
			_isSwinging = false;
		}
	}

	// 再生時間をセットする
	MV1SetAttachAnimTime(_modelHandle, _attach_index, _play_time);
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

	DrawDebugInfo();
	return true;
}



void Player::UpdateCollision()
{
	_capsuleCollision.down_pos = _pos;
	_capsuleCollision.r = 35.0f;
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

void Player::DrawDebugInfo()
{
	for (int i = 0; i < sizeof(_bone) / sizeof(_bone[0]); i++) {
		_bone[i]->DebugRender();
	}
	//DrawCapsule3D(_capsuleCollision._startPos, _capsuleCollision._endPos, _capsuleCollision._r, 16, COLOR_RED, COLOR_RED, false);
}
