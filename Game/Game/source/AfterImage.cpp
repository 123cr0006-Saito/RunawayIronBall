//----------------------------------------------------------------------
// @filename AfterImage.cpp
// @date: 2024/04/26
// @author: Morozumi Hiroya
// @explanation
// キャラクターの残像表現を行うクラス
// 半透明にしたモデルを複数体表示することで残像を表現する
//----------------------------------------------------------------------
#include "AfterImage.h"

AfterImage::AfterImage()
{
	_parentModelHandle = -1;

	_modelInfo.clear();
	_afterImageNum = 0;
	_remainTimeMax = 0;
}

AfterImage::~AfterImage()
{
	for(auto itr = _modelInfo.begin(); itr != _modelInfo.end(); ++itr)
	{
		delete (*itr);
		(*itr) = nullptr;
	}
	_modelInfo.clear();
}

// 初期化処理
// @param parentModelHandle: 親モデルのハンドル
// @param keyName: キー名（ResourceServerでのモデルデータの管理に使用する）
// @param modelName: モデルのパス
// @param afterImageNum: 同時に表示する残像の最大数
// @param remainTime: 残像の持続時間
void AfterImage::Init(int parentModelHandle, std::string keyName, std::string modelName, int afterImageNum, int remainTime)
{
	_parentModelHandle = parentModelHandle;

	this->_afterImageNum = afterImageNum;
	_modelInfo.resize(afterImageNum);

	this->_remainTimeMax = remainTime;

	// 残像モデル情報の初期化
	for(int i = 0; i < _afterImageNum; i++) {
		_modelInfo[i] = new ModelInfo();

		_modelInfo[i]->use = false;
		_modelInfo[i]->remainTime = 0;

		_modelInfo[i]->modelHandle = ResourceServer::MV1LoadModel(keyName, modelName);
		_modelInfo[i]->difColorScale = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

// 残像を追加する
// @param animIndex: アニメーションのインデックス（mv1ファイル内でのインデックス番号）（-1の場合はアニメーションを適応しない）
// @param playTime: アニメーションの再生時間
void AfterImage::AddAfterImage(int animIndex, float playTime)
{
	for(int i = 0; i < _afterImageNum; i++) {
		// 使用していない要素を探す
		if(_modelInfo[i]->use == false) {
			// 残像の設定
			_modelInfo[i]->use = true;
			_modelInfo[i]->remainTime = _remainTimeMax;

			// 親モデルのディフューズカラーのスケール値を取得し、残像モデルに適応する
			_modelInfo[i]->difColorScale = MV1GetDifColorScale(_parentModelHandle);
			MV1SetDifColorScale(_modelInfo[i]->modelHandle, _modelInfo[i]->difColorScale);

			// 親モデルの行列を残像モデルに適応する
			MV1SetMatrix(_modelInfo[i]->modelHandle, MV1GetMatrix(_parentModelHandle));

			// アニメーションの設定
			if(animIndex != -1) {
				_modelInfo[i]->attachIndex = MV1AttachAnim(_modelInfo[i]->modelHandle, animIndex);
				MV1SetAttachAnimTime(_modelInfo[i]->modelHandle, _modelInfo[i]->attachIndex, playTime);
			}

			break;
		}
	}
}

// 更新処理
void AfterImage::Process()
{
	for (int i = 0; i < _afterImageNum; i++) {
		if (_modelInfo[i]->use == true) {
			// 残りカウントを減らす
			_modelInfo[i]->remainTime--;

			// 残りカウントが0以下になったら使用フラグをfalseにする
			if (_modelInfo[i]->remainTime <= 0) {
				_modelInfo[i]->use = false;

				// アニメーションのデタッチ
				MV1DetachAnim(_modelInfo[i]->modelHandle, _modelInfo[i]->attachIndex);
			}
			else {
				// 残りカウントに応じて透明度を変更する
				float alphaRate = (float)_modelInfo[i]->remainTime / (float)_remainTimeMax;
				COLOR_F nextColor = _modelInfo[i]->difColorScale;
				nextColor.a = _modelInfo[i]->difColorScale.a * alphaRate;
				MV1SetDifColorScale(_modelInfo[i]->modelHandle, nextColor);
			}
		}
	}
}

// 描画処理
void AfterImage::Render()
{
	for(int i = 0; i < _afterImageNum; i++) {
		if(_modelInfo[i]->use == true) {
			MV1DrawModel(_modelInfo[i]->modelHandle);
		}
	}
}

