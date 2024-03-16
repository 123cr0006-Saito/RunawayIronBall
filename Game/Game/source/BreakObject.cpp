#include "BreakObject.h"

BreakObject::BreakObject()
{
	_isActive = false;
	_finishedBreakAll = false;
	

	_modelHandle = -1;
	//MV1SetPosition(_modelHandle, _pos);
	//MV1SetScale(_modelHandle, VGet(10.0f, 10.0f, 10.0f));

	//_breakFrameIndex = 35;

	_breakCnt = 0;

	_blastDir = VGet(0.0f, 0.0f, -1.0f);


	_isDrawLocus = false;
}

BreakObject::~BreakObject()
{
	for (auto&& frame : _frameInfo) {
		delete frame; frame = nullptr;
	}
}

void BreakObject::Init(int modelHandle)
{
	_modelHandle = modelHandle;
	for (int i = 0; i < MV1GetFrameNum(_modelHandle); i++) {

		// フレームの名前を取得する
		std::string frameName = MV1GetFrameName(_modelHandle, i);
		std::string checkName = "BRK";
		// フレーム名がBRKで始まらない場合はcontinue
		if (frameName.substr(0, 3) != checkName) {
			continue;
		}


		// フレームの座標変換行列を取得する
		MATRIX mFrameLocal = MV1GetFrameLocalMatrix(_modelHandle, i);
		// フレームのローカル座標を求める
		VECTOR vFrameLocalPos = VGet(0.0f, 0.0f, 0.0f);
		vFrameLocalPos = VTransform(vFrameLocalPos, mFrameLocal);

		// モデルの起点座標から見たフレームの方向を取得する
		VECTOR vFrameLocalDir = VGet(0.0f, 0.0f, -1.0f);
		if (VSquareSize(vFrameLocalPos) > 0.0000f) {
			vFrameLocalDir = VNorm(vFrameLocalPos);
			// 吹っ飛ぶ方向をz軸マイナス方向に限定する
			if (vFrameLocalDir.z > 0.0f) {
				vFrameLocalDir.z *= -1;
			}
		}

		FRAME_INFO* f = NEW FRAME_INFO();
		f->frameIndex = i;
		f->finishedBreak;
		f->horizontalDir = vFrameLocalDir;
		f->horizontalVelocity = 0.0f;
		f->verticalVelocity = 0.0f;
		f->vRot = VGet(0.0f, 0.0f, 0.0f);

		_frameInfo.push_back(f);





		_locus.resize(_locus.size() + 1);
	}






	// デバッグ用

	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		MATRIX m = MV1GetFrameLocalWorldMatrix(_modelHandle, (*itr)->frameIndex);

		VECTOR v = VTransform(VGet(0.0f, 0.0f, 0.0f), m);
		_locus.at(std::distance(_frameInfo.begin(), itr)).push_back(v);
	}

	// 吹っ飛ばす方向を指定
	SetBlastDir(VGet(1.0f, 0.0f, 0.0f));
}

void BreakObject::Process()
{
	if (!_finishedBreakAll && _isActive) {
		_finishedBreakAll = true;
		// 破片が飛び散る処理
		for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {

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



			// 重力処理
			(*itr)->verticalVelocity -= 2.f;

			// 軌跡表示用の座標を保持
			MATRIX mLocus = MV1GetFrameLocalWorldMatrix(_modelHandle, (*itr)->frameIndex);
			VECTOR vLocus = VTransform(VGet(0.0f, 0.0f, 0.0f), mLocus);
			_locus.at(std::distance(_frameInfo.begin(), itr)).push_back(vLocus);

			if (vLocus.y < 0.0f) {
				(*itr)->finishedBreak = true;
			}

			//itr->pos = v;
		}
		//_blastPower -= 0.5f;

		_breakCnt++;
		//_blastDir.y -= 0.05f;

		// // リセット
		//if (_breakCnt > 180) {
		//	_finishedBreakAll = true;

		//	//_breakCnt = 0;

		//	//// 吹っ飛ばす方向を指定
		//	//SetBlastDir(VGet(0.0f, 0.0f, 1.0f));

		//	//// 軌跡表示用の座標情報をリセット
		//	//for (auto itr = _locus.begin(); itr != _locus.end(); ++itr) {
		//	//	itr->clear();
		//	//}

		//	//// パーツを初期位置に戻す
		//	//for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		//	//	MV1ResetFrameUserLocalMatrix(_modelHandle, itr->frameIndex);

		//	//	MATRIX m = MV1GetFrameLocalWorldMatrix(_modelHandle, itr->frameIndex);
		//	//	VECTOR v = VTransform(VGet(0.0f, 0.0f, 0.0f), m);
		//	//	_locus.at(std::distance(_frameInfo.begin(), itr)).push_back(v);
		//	//}
		//}


	}
	// 軌跡表示のOn/Off切り替え
	auto input = XInput::GetInstance();
	if (input->GetTrg(XINPUT_BUTTON_BACK) != 0) {
		_isDrawLocus = !_isDrawLocus;
	}
}

void BreakObject::Render()
{

}

void BreakObject::Activate(bool activate, VECTOR _blastDir)
{
	if (!activate) {
		ResetFrameMatrix();
	}else if (!_isActive) { // 現在が有効状態でなく、新しく有効化する場合にのみ吹っ飛ばしの設定を行う	
		SetBlastDir(_blastDir);
	}
	_isActive = activate;
}

// 吹っ飛ばしの方向をセットする
// 引数
// vDir : ふっ飛ばしの中心方向
void BreakObject::SetBlastDir(VECTOR vDir)
{
	_modelRotation = MV1GetRotationXYZ(_modelHandle);


	vDir.y = 0.0f;
	vDir = VNorm(vDir);

	// ふっ飛ばし方向をモデルの回転値に合わせてローカル座標系に変換する
	MATRIX mToLocal = MGetRotY(-_modelRotation.y);
	VECTOR vLocalDir = VTransform(vDir, mToLocal);

	// パーツごとに吹っ飛ばす水平方向をvDirから ±maxRange度の間でランダムに決定する
	const int maxRange = 35;
	// 水平・鉛直方向における最大速度
	const int maxHorizontalVelocity = 80;
	const int maxVerticalVelocity = 50;

	// パーツごとにふっ飛ばし方向をセットする


	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		// 水平方向
		float angle = rand() % (maxRange * 2);
		angle -= maxRange;
		

		(*itr)->horizontalDir = VTransform(vLocalDir, MGetRotY(Math::DegToRad(angle)));
		(*itr)->horizontalVelocity = rand() % maxHorizontalVelocity;

		// 鉛直方向
		(*itr)->verticalVelocity = (rand() % maxVerticalVelocity) + 20.0f;

		// 回転値
		int deltaRot = 4;

		float angleX = rand() % deltaRot;
		angleX = Math::DegToRad(angleX);
		int mul = rand() % 2 == 0 ? 1 : -1;
		angleX *= mul;

		float angleY = rand() % deltaRot;
		angleY = Math::DegToRad(angleY);
		mul = rand() % 2 == 0 ? 1 : -1;
		angleY *= mul;

		float angleZ = rand() % deltaRot;
		angleZ = Math::DegToRad(angleZ);
		mul = rand() % 2 == 0 ? 1 : -1;
		angleZ *= mul;

		(*itr)->vRot = VGet(angleX, angleY, angleZ);
	}
	//_blastPower = 20.0f;
	_blastDir = vDir;
}

void BreakObject::ResetFrameMatrix()
{
	for (auto itr = _frameInfo.begin(); itr != _frameInfo.end(); ++itr) {
		MV1ResetFrameUserLocalMatrix(_modelHandle, (*itr)->frameIndex);
	}
}

void BreakObject::DrawDebugInfo()
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
