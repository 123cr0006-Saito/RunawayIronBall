#include "bone.h"

const Vector3D bone::_orign(0,0,0);

const float bone::_massWeight = 1.5f;
const float bone::_viscousResistance = 20.0f;
const float bone::_gravity = 4000.0f;
const float bone::_spring = 800.0f;

const float bone::_naturalCorrectionFactor = 0.8f;
const Vector3D bone::_gravityDir(0.0f, -1.0f, 0.0f);

//1Ｆ 大体0.015 ~ 0.017秒ぐらい 
//1回だと発散する
//2回だとぐにゃぐにゃになる
//10回だと鎖っぽくなる
//100回以上がきれいに見える

//今は大体150～170回
const double bone::_processInterval = 0.0001;

bone::bone(
	int* Model,
	std::vector<int> list,
	int size
) :
	_model(Model),
	_frameList(list),
	_listSize(size),
	_springList(size + 1),
	_naturalList(size + 1)
{
	//ボーンの初期化
	_vecDirList = new Vector3D[_listSize];
	_orignPos = new Vector3D[_listSize + 2];
	_transMatrixList = new MATRIX[_listSize];


	for (int i = 0; i < _listSize; i++) {
		_transMatrixList[i] = MV1GetFrameLocalMatrix(*_model, _frameList[i + 1]);
		MATRIX local_mat = MV1GetFrameLocalMatrix(*_model, _frameList[i + 2]);
		_vecDirList[i] = VTransform(_orign.toVECTOR(), local_mat);

		for (int j = 0; j < 3; j++) {
			_transMatrixList[i].m[3][j] = 0.0f;
		}

		_orignPos[i + 2] = MV1GetFramePosition(*_model, _frameList[i + 2]);
	}
	_orignPos[0] = MV1GetFramePosition(*_model, _frameList[0]);
	_orignPos[1] = MV1GetFramePosition(*_model, _frameList[1]);

	//----------------------------------------------------------------------------------
	//物理演算をするための変数の初期化
	_massPointSize = _frameList.size() - 1;
	_massPosList = new Vector3D[_massPointSize];
	_massAccelList = new Vector3D[_massPointSize];

	for (int i = 0; i < _massPointSize; i++) {
		_massPosList[i].Set(MV1GetFramePosition(*_model, _frameList[i + 1]));
	}

	for (int i = 0; i < _massPointSize - 1; i++) {
		//_naturalListの初期化
		//_natulalCorrectionFactorは通常1.0で良いと思うが、
		// 髪の毛が異常に長くなったときは_natulalCorrectionFactorを変更し
		// 元のモデルの髪の長さに近い状態にする
		_naturalList[i] = _naturalCorrectionFactor * (_massPosList[i + 1] - _massPosList[i]).Len();

		//_springListの初期化
		//ばねの値は決まっているが_naturalCorrectionFactorに比例して変更する
		//今回は質点が一定の長さだが必要ないが
		// 質点に合わしてばね定数も変更可能
		_springList[i] = _naturalList[i] * _spring;
	}
};

bone::~bone() {
	_model = nullptr;
	delete[] _vecDirList;         _vecDirList = nullptr;
	delete[] _orignPos;           _orignPos = nullptr;
	delete[] _transMatrixList; _transMatrixList = nullptr;
	delete[] _massPosList;    _massPosList = nullptr;
	delete[] _massAccelList;  _massAccelList = nullptr;
};

void bone::SetMain(Vector3D* pos_list) {
	for (int i = 0; i < _listSize; i++) {
		SetBoneDir(
			pos_list[i + 1],
			pos_list[i],
			_frameList[i + 1],
			_frameList[i],
			_transMatrixList[i],
			_vecDirList[i]
		);
	};
};

void bone::SetBoneDir(
	Vector3D world_dir_vec,
	Vector3D boon_pos,
	int target_frame,
	int parent_frame,
	MATRIX trans_mat,
	Vector3D dir_parent)
{
	MATRIX tmpMat;
	tmpMat = MV1GetFrameLocalWorldMatrix(*_model, parent_frame); // 親ボーンのローカル→ワールド
	tmpMat = MInverse(tmpMat); // 逆行列を計算
	Vector3D localDirVec = VTransform(world_dir_vec.toVECTOR(), tmpMat);//ボーンを方向のローカル座標
	Vector3D localBonePos = VTransform(boon_pos.toVECTOR(), tmpMat);//自分の付け根のローカル座標

	//ボーンを向けたい方向
	Vector3D localDirLook = (localDirVec - localBonePos).Normalize();

	//今の向きから次の向きへの回転行列
	MATRIX rotationMat = MGetRotVec2(dir_parent.toVECTOR(), localDirLook.toVECTOR());

	//自分の付け根が位置になるよう、平行移動
	MATRIX shiftPos = trans_mat;
	shiftPos.m[3][0] = localBonePos.x;
	shiftPos.m[3][1] = localBonePos.y;
	shiftPos.m[3][2] = localBonePos.z;

	tmpMat = MMult(rotationMat, shiftPos);
	MV1SetFrameUserLocalMatrix(*_model, target_frame, tmpMat);
}

bool bone::Process() {

	double _elapsedTime = global._timer->GetElapsedTime();
	while (1)
	{
		//1フレームを_processIntervalで差分化する
		if (_elapsedTime < _processInterval)break;
		_elapsedTime -= _processInterval;
		UpdatePosAndAccel(_processInterval);
	}

	return true;
};

//参考サイト
//http://www.den.t.u-tokyo.ac.jp/ad_prog/ode/ //オイラー法について
//https://high-school-physics.com/spring-constant-of-the-combined-spring/ //ばねのつり合いについて
//オイラー法で計算したため、ひとつ前の計算した値を使用している
//時間があればルンゲクッタ法に変更したい
void bone::UpdatePosAndAccel(double _elapsedTime) {
	//時間で処理を細分化し少しずつ答えに近づけていく
	static Vector3D* newPosList = new Vector3D[_massPointSize];
	static Vector3D* newAccelList = new Vector3D[_massPointSize];

	//付け根の位置は固定
	_massPosList[0] = MV1GetFramePosition(*_model, _frameList[1]);
	newPosList[0] = _massPosList[0];

	// 速度と位置の更新
	for (int i = 1; i < _massPointSize; i++) {
		// ニュートンの運動方程式より
		// F = ma 今回は速度が欲しいので a = F/m
		Vector3D Accel = ForceWorksToMassPoint(i, _massPosList, _massAccelList) / _massWeight;
		//速度を出す   y(i+1) = y(i) + hf 
		newAccelList[i] = _massAccelList[i] + _elapsedTime * Accel;
		//位置の更新   y(i+1) = y(i) + hf 
		newPosList[i] = _massPosList[i] + _elapsedTime * _massAccelList[i];
	}

	// 速度と位置をまとめて変更
	std::swap(_massAccelList, newAccelList);
	std::swap(_massPosList, newPosList);
};

//質点に働く力を計算 F=maを求める
//参考サイト
//https://www.yukimura-physics.com/entry/dyn-f22 //運動方程式の立て方について
Vector3D bone::ForceWorksToMassPoint(int i, Vector3D* posList, Vector3D* accelList) {
	Vector3D force;

	//行う処理
	//ばねのつり合い 上向きの弾性力と下向きの重力がつり合いを計算
	//抵抗をつける
	//重力をかける

	//質点i～質点i+1間のばねから受ける力
	if (i < _massPointSize - 1) {
		////ばねの伸び具合を調べる
		float growth = (posList[i + 1] - posList[i]).Len() - _naturalList[i];
		//ばねの伸びを力に変換
		force += _springList[i] * growth * (posList[i + 1] - posList[i]).Normalize();
	}

	// 質点i-1～質点i間のバネから受ける力 
	//ばねの伸び具合を調べる
	float growth = (posList[i] - posList[i - 1]).Len() - _naturalList[i - 1];
	//ばねの伸びを力に変換
	force += _springList[i - 1] * growth * (posList[i - 1] - posList[i]).Normalize();

	//今回は粘性抵抗 
	//抵抗力なので－を足す
	force -= _viscousResistance * accelList[i];

	//重力 
	force += _massWeight * _gravity * _gravityDir;

	return force;
};

void bone::PositionReset() {
	//※注意　位置や速度を初期化しますが、
	// 垂直なモデルでないと初期化した後重力の影響で動きます
	for (int i = 0; i < _listSize; i++) {
		//座標返還行列の初期化
		MV1ResetFrameUserLocalMatrix(*_model, _frameList[i + 1]);
	}

	for (int i = 0; i < _massPointSize; i++) {
		//位置の初期化
		_massPosList[i] = MV1GetFramePosition(*_model, _frameList[i + 1]);
		//速度を０にする
		_massAccelList[i].Set(0.0f, 0.0f, 0.0f);
	}
};