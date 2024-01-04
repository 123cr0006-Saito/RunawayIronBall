#include "bone.h"

const VECTOR bone::orign = VGet(0, 0, 0);

const float bone::_massWeight = 0.2f;
const float bone::_viscousResistance = 1.5f;
const float bone::_gravity = 400.0f;
const float bone::_spring = 50.0f;
const float bone::_naturalCorrectionFactor = 1.0f;

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
	model(Model),
	Frame_list(list),
	list_size(size),
	_springList(size + 1),
	_naturalList(size + 1)
{
	//ボーンの初期化
	vec_dir_list = new VECTOR[list_size];
	orign_pos = new VECTOR[list_size + 2];
	trans_mat_list = new MATRIX[list_size];


	for (int i = 0; i < list_size; i++) {
		trans_mat_list[i] = MV1GetFrameLocalMatrix(*model, Frame_list[i + 1]);
		MATRIX local_mat = MV1GetFrameLocalMatrix(*model, Frame_list[i + 2]);
		vec_dir_list[i] = VTransform(orign, local_mat);

		for (int j = 0; j < 3; j++) {
			trans_mat_list[i].m[3][j] = 0.0f;
		}
		orign_pos[i + 2] = MV1GetFramePosition(*model, Frame_list[i + 2]);
	}
	orign_pos[0] = MV1GetFramePosition(*model, Frame_list[0]);
	orign_pos[1] = MV1GetFramePosition(*model, Frame_list[1]);

	//----------------------------------------------------------------------------------
	//物理演算をするための変数の初期化
	_massPointSize = Frame_list.size() - 1;
	_massPosList = new Vector3D[_massPointSize];
	_massAccelList = new Vector3D[_massPointSize];

	for (int i = 0; i < _massPointSize; i++) {
		_massPosList[i].Set(MV1GetFramePosition(*model, Frame_list[i + 1]));
	}

	for (int i = 0; i < _massPointSize - 1; i++) {
		//_naturalListの初期化
		//_natulalCorrectionFactorは通常1.0で良いと思うが、
		// 髪の毛が異常に長くなったときは_natulalCorrectionFactorを変更し
		// 元のモデルの髪の長さに近い状態にする
		_naturalList[i] = _naturalCorrectionFactor * (_massPosList[i + 1] - _massPosList[i]).len();

		//_springListの初期化
		//ばねの値は決まっているが_naturalCorrectionFactorに比例して変更する
		//今回は質点が一定の長さだが必要ないが
		// 質点に合わしてばね定数も変更可能
		_springList[i] = _naturalList[i] * _spring;
	}
};

bone::~bone() {
	SAFE_DELETE(vec_dir_list);
	SAFE_DELETE(orign_pos);
	SAFE_DELETE(trans_mat_list);
	model = nullptr;
};

void bone::SetMain(Vector3D* pos_list) {
	for (int i = 0; i < list_size; i++) {
		SetBoneDir(
			pos_list[i + 1].toVECTOR(),
			pos_list[i].toVECTOR(),
			Frame_list[i + 1],
			Frame_list[i],
			trans_mat_list[i],
			vec_dir_list[i]
		);
	};
};

void bone::SetBoneDir(
	VECTOR world_dir_vec,
	VECTOR boon_pos,
	int target_frame,
	int parent_frame,
	MATRIX trans_mat,
	VECTOR dir_parent)
{
	MATRIX TmpMat;
	TmpMat = MV1GetFrameLocalWorldMatrix(*model, parent_frame); // 親ボーンのローカル→ワールド
	TmpMat = MInverse(TmpMat); // 逆行列を計算
	VECTOR Local_Dir_Vec = VTransform(world_dir_vec, TmpMat);//ボーンを方向のローカル座標
	VECTOR Local_Bone_Pos = VTransform(boon_pos, TmpMat);//自分の付け根のローカル座標

	//ボーンを向けたい方向
	VECTOR Local_Dir_Look = VNorm(VSub(Local_Dir_Vec, Local_Bone_Pos));

	//今の向きから次の向きへの回転行列
	MATRIX RotMat = MGetRotVec2(dir_parent, Local_Dir_Look);

	//自分の付け根が位置になるよう、平行移動
	MATRIX shift_point = trans_mat;
	shift_point.m[3][0] = Local_Bone_Pos.x;
	shift_point.m[3][1] = Local_Bone_Pos.y;
	shift_point.m[3][2] = Local_Bone_Pos.z;

	TmpMat = MMult(RotMat, shift_point);
	MV1SetFrameUserLocalMatrix(*model, target_frame, TmpMat);
}

bool bone::Process() {

	double _elapsedTime = global._timer->GetElapsedTime();
	//int processCount = 0;
	while (1)
	{
		if (_elapsedTime < _processInterval)break;
		_elapsedTime -= _processInterval;
		UpdatePosAndAccel(_processInterval);
		//processCount++;
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
	Vector3D* newPosList = new Vector3D[_massPointSize];
	Vector3D* newAccelList = new Vector3D[_massPointSize];

	//付け根の位置は固定
	_massPosList[0] = MV1GetFramePosition(*model, Frame_list[1]);
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
	Vector3D _force;

	//行う処理
	//ばねのつり合い 上向きの弾性力と下向きの重力がつり合いを計算
	//抵抗をつける
	//重力をかける

	//質点i～質点i+1間のばねから受ける力
	if (i < _massPointSize - 1) {
		//ばねの伸び具合を調べる
		float _growth = (posList[i + 1] - posList[i]).len() - _naturalList[i];
		//ばねの伸びを力に変換
		_force += _springList[i] * _growth * (posList[i + 1] - posList[i]).normalize();
	}

	// 質点i-1～質点i間のバネから受ける力 
	//ばねの伸び具合を調べる
	float _growth = (posList[i] - posList[i - 1]).len() - _naturalList[i - 1];
	//ばねの伸びを力に変換
	_force += _springList[i - 1] * _growth * (posList[i - 1] - posList[i]).normalize();

	//今回は粘性抵抗 
	//抵抗力なので－を足す
	_force -= _viscousResistance * accelList[i];

	//重力 
	_force += _massWeight * _gravity * _gravityDir;

	return _force;
};