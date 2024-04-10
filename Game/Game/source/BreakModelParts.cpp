//----------------------------------------------------------------------
// @filename BreakModelParts.cpp
// ＠date: 2024/04/01
// ＠author: Morozumi Hiroya
// @explanation
// 建物モデルをパーツごとに吹き飛ばす処理を行うクラス
// あらかじめモデルのパーツを分割しておき、そのパーツ分けを利用して吹き飛ばす処理を行う
// パーツの名前の先頭に「BRK」がついているものを吹き飛ばす対象とし、それ以外のパーツはその場にとどまる 
//----------------------------------------------------------------------
#include "BreakModelParts.h"

BreakModelParts::BreakModelParts()
{
	_isActive = false;
	_finishedBreakAll = false;

	_modelHandle = -1;
	_modelRotation = VGet(0.0f, 0.0f, 0.0f);

	_blastDir = VGet(0.0f, 0.0f, -1.0f);

	_isDrawLocus = false;
}

BreakModelParts::~BreakModelParts()
{
	for (auto&& frame : _frameInfo) {
		delete frame; frame = nullptr;
	}
}

void BreakModelParts::Init(int modelHandle)
{
	_modelHandle = modelHandle;
	for (int i = 0; i < MV1GetFrameNum(_modelHandle); i++) {

		// フレームの名前を取得する
		std::string frameName = MV1GetFrameName(_modelHandle, i);
		std::string checkName = "BRK";
		// フレーム名が「BRK」で始まらない場合はcontinue
		if (frameName.substr(0, 3) != checkName) {
			continue;
		}

		FRAME_INFO* f = NEW FRAME_INFO();
		f->frameIndex = i;
		f->finishedBreak = false;
		f->horizontalDir = VGet(0.0f, 0.0f, -1.0f);
		f->horizontalVelocity = 0.0f;
		f->verticalVelocity = 0.0f;
		f->vRot = VGet(0.0f, 0.0f, 0.0f);

		_frameInfo.push_back(f);
	}

	// デバッグ用
	_locus.resize(_frameInfo.size());
	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		MATRIX m = MV1GetFrameLocalWorldMatrix(_modelHandle, (*itr)->frameIndex);

		VECTOR v = VTransform(VGet(0.0f, 0.0f, 0.0f), m);
		_locus.at(std::distance(_frameInfo.begin(), itr)).push_back(v);
	}
}

// 更新処理
void BreakModelParts::Process()
{
	// 吹き飛ばし処理
	// 有効状態かつ、吹き飛ばし処理が終了していないパーツがある場合
	if (!_finishedBreakAll && _isActive) {
		_finishedBreakAll = true;
		// 破片が飛び散る処理
		for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
			// すでに吹き飛ばし処理が終了している場合はスキップ
			if((*itr)->finishedBreak) continue;
			_finishedBreakAll = false;

			// 回転行列
			MATRIX mRot = MGetRotX((*itr)->vRot.x);
			mRot = MMult(mRot, MGetRotY((*itr)->vRot.y));
			mRot = MMult(mRot, MGetRotZ((*itr)->vRot.z));

			// 移動前の行列
			MATRIX mBefor = MV1GetFrameLocalMatrix(_modelHandle, (*itr)->frameIndex);
			// 平行移動行列（水平方向と鉛直方向の平行移動行列を合成する）
			MATRIX mTrans = MGetTranslate(VAdd(VScale((*itr)->horizontalDir, (*itr)->horizontalVelocity), VGet(0.0f, (*itr)->verticalVelocity, 0.0f)));

			MATRIX m = MMult(mRot, mBefor);
			m = MMult(m, mTrans);

			// 行列の適応
			MV1SetFrameUserLocalMatrix(_modelHandle, (*itr)->frameIndex, m);


			// パーツ吹き飛ばし処理の終了判定
			MATRIX mPivot = MV1GetFrameLocalWorldMatrix(_modelHandle, (*itr)->frameIndex);
			VECTOR pivotPos = VTransform(VGet(0.0f, 0.0f, 0.0f), mPivot);
			// パーツごとのピボット座標が地面より下にある場合、吹き飛ばし処理を終了する
			if (pivotPos.y < 0.0f) {
				(*itr)->finishedBreak = true;
			}

			// 重力加速
			(*itr)->verticalVelocity -= 2.f;



			// 軌跡表示用の座標を保持
			_locus.at(std::distance(_frameInfo.begin(), itr)).push_back(pivotPos);
		}
	}

	// 軌跡表示のOn/Off切り替え
	auto input = XInput::GetInstance();
	if (input->GetTrg(XINPUT_BUTTON_BACK) != 0) {
		_isDrawLocus = !_isDrawLocus;
	}
}

// 吹っ飛び処理の有効化設定
// @param activate : 吹っ飛び処理を有効化するかどうか
// @param _blastDir : 吹っ飛ばしの方向
void BreakModelParts::Activate(bool activate, VECTOR blastDir)
{
	if (!activate) {
		ResetFrameMatrix();
	}else if (!_isActive) { // 現在が有効状態でなく、新しく有効化する場合にのみ吹っ飛ばしの設定を行う	
		SetBlastDir(blastDir);
	}
	_isActive = activate;
}

// 吹っ飛ばしの方向をセットする
// パーツごとに、吹っ飛ばしの方向と1フレームあたりの回転地を設定する
// @param vDir : 吹っ飛ばしの中心方向
void BreakModelParts::SetBlastDir(VECTOR vDir)
{
	_modelRotation = MV1GetRotationXYZ(_modelHandle);

	// XZ平面上で方向を指定する
	vDir.y = 0.0f;
	vDir = VNorm(vDir);

	// ふっ飛ばし方向をモデルの回転値に合わせてローカル座標系に変換する
	MATRIX mToLocal = MGetRotY(-_modelRotation.y);
	VECTOR vLocalDir = VTransform(vDir, mToLocal);

	// 吹っ飛ばしの水平方向パーツごとに、vLocalDirから±maxRange度の間でランダムに決定する
	const int maxRange = 35;
	// 水平・鉛直方向における最大速度
	const int maxHorizontalVelocity = 80;
	const int maxVerticalVelocity = 50;

	// パーツごとにふっ飛ばし方向と速度をセットする
	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		// ------------------------------
		// 水平方向の設定

		// vLocalDirから一定範囲内でランダムに方向を決定する
		float angle = rand() % (maxRange * 2);
		angle -= maxRange;
		// 方向
		(*itr)->horizontalDir = VTransform(vLocalDir, MGetRotY(Math::DegToRad(angle)));
		// 速度
		(*itr)->horizontalVelocity = rand() % maxHorizontalVelocity;
		(*itr)->horizontalVelocity = Math::Clamp(20.0f, maxHorizontalVelocity, (*itr)->horizontalVelocity);
		// ------------------------------

		// ------------------------------
		// 鉛直方向の設定

		// 速度
		(*itr)->verticalVelocity = rand() % maxVerticalVelocity;
		(*itr)->verticalVelocity = Math::Clamp(20.0f, maxVerticalVelocity, (*itr)->verticalVelocity);
		// ------------------------------

		// ------------------------------
		// 1フレーム当たりの回転値を軸ごとにランダムに決定する

		// 回転値の最大
		int deltaRotMax = 4;

		// X軸の回転値
		float angleX = rand() % deltaRotMax;
		angleX = Math::DegToRad(angleX);
		// 回転方向をランダムに決定
		int mul = rand() % 2 == 0 ? 1 : -1;
		angleX *= mul;

		// Y軸の回転値
		float angleY = rand() % deltaRotMax;
		angleY = Math::DegToRad(angleY);
		// 回転方向をランダムに決定
		mul = rand() % 2 == 0 ? 1 : -1;
		angleY *= mul;

		// Z軸の回転値
		float angleZ = rand() % deltaRotMax;
		angleZ = Math::DegToRad(angleZ);
		// 回転方向をランダムに決定
		mul = rand() % 2 == 0 ? 1 : -1;
		angleZ *= mul;

		// 1フレームあたりの回転値をセット
		(*itr)->vRot = VGet(angleX, angleY, angleZ);
		// ------------------------------
	}
	_blastDir = vDir;
}

// パーツごとに適応した行列をリセットし、モデルを初期状態に戻す
void BreakModelParts::ResetFrameMatrix()
{
	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		MV1ResetFrameUserLocalMatrix(_modelHandle, (*itr)->frameIndex);
	}
}

// デバッグ情報の表示
void BreakModelParts::DrawDebugInfo()
{
	if (_isActive && _isDrawLocus) {
		// ふっ飛ばし方向の中心
		{
			VECTOR startPos = MV1GetPosition(_modelHandle);
			DrawLine3D(startPos, VAdd(startPos, VScale(_blastDir, 1000.0f)), GetColor(0, 255, 0));
		}

		// パーツごとに吹っ飛びの軌跡を表示する
		for (auto i = _locus.begin(); i != _locus.end(); ++i) {
			for (auto j = i->begin(); j != i->end() - 1; ++j) {
				DrawLine3D((*j), (*(j + 1)), GetColor(255, 255, 0));
			}
		}
	}
}
