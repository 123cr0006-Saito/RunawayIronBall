#include "Camera.h"
Camera::Camera() {
	this->_input = XInput::GetInstance();//コンストラクタで入力用のクラスのポインタを取得

	_cameraDirX = 0.53f;
	_cameraDirY = 0.0f;
	_pointDistance = VGet(0, 0, -2360);
	_gazeShift = VGet(0, 80, 0);
	_reverseX = -1;
	_reverseY = 1;
};

Camera::~Camera() {
	_input = nullptr;
};

bool Camera::Process(VECTOR pos) {

	//入力から得た値で移動値を返す関数
	auto move_speed_process = [](float pos, float pos_max, float max_speed) {return pos * max_speed / pos_max; };

	//32768はshort型の最大値 移動速度の最大は0.02
	if (_input->GetRx() != 0) {
		_cameraDirY += move_speed_process(_input->GetRx(), 32768, 0.02) * _reverseY;
	}
	if (_input->GetRy() != 0) {
		_cameraDirX += move_speed_process(_input->GetRy(), 32768, 0.02) * _reverseX;
	}

	//デバック用
	// 十字キーで微調整できる

	//if (input->GetKey(XINPUT_BUTTON_DPAD_DOWN)) {
	//	_cameraDirY -= 0.02;
	//}
	//if (input->GetKey(XINPUT_BUTTON_DPAD_UP)) {
	//	_cameraDirY += 0.02;
	//}
	//if (input->GetKey(XINPUT_BUTTON_DPAD_LEFT)) {
	//	camera_dir_x -= 0.02;
	//}
	//if (input->GetKey(XINPUT_BUTTON_DPAD_RIGHT)) {
	//	_cameraDirX += 0.02;
	//}

	//トリガ入力でカメラの距離を変更
	//カメラが遠くに移動
	if (_input->GetRTrg() > 25) {
		if (_pointDistance.z > -2500) {
			_pointDistance.z -= _input->GetRTrg() / 25;
		}
	}
	//カメラが近くに移動
	if (_input->GetLTrg() > 25) {
		if (_pointDistance.z < -150) {
			_pointDistance.z += _input->GetLTrg() / 25;
		}
	}

	//±で1周回ったら０度に変換
	if (abs(Math::RadToDeg(_cameraDirX)) > 360) {
		_cameraDirX = 0;
	}
	if (abs(Math::RadToDeg(_cameraDirY)) > 360) {
		_cameraDirY = 0;
	}

	if (_cameraDirX >= 1.39491415f) {
		_cameraDirX = 1.39491415f;
	}
	else if (_cameraDirX <= -1.39491415f) {
		_cameraDirX = -1.39491415f;
	}

	//カメラの位置を計算
	MATRIX origin = MGetIdent();
	MATRIX MatrixX = MGetRotX(_cameraDirX);
	MATRIX MatrixY = MGetRotY(_cameraDirY);

	//引数としてプロセス内で得るのはめんどくさそいので
	//できればプレイヤークラスから引っ張ってきたいです
	VECTOR playerPos = pos;

	//行列の掛け算
	origin = MMult(origin, MatrixX);
	origin = MMult(origin, MatrixY);

	//注視点からの距離に行列を変換する
	VECTOR Vecter = VTransform(_pointDistance, origin);

	//注視点の位置に移動
	VECTOR VecAdd = VAdd(Vecter, playerPos);
	//カメラのセット
	SetCameraPositionAndTarget_UpVecY(VecAdd, VAdd(playerPos, _gazeShift));

	return true;
};