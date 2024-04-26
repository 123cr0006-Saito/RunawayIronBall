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
	_afterImageNum = 0;
	_modelInfo.clear();
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
// @param keyName: キー名（ResourceServerでのモデルデータの管理に使用する）
// @param modelName: モデルのパス
// @param afterImageNum: 同時に表示する残像の最大数
void AfterImage::Init(int parentModelHandle, std::string keyName, std::string modelName, int afterImageNum)
{
	_parentModelHandle = parentModelHandle;

	this->_afterImageNum = afterImageNum;
	_modelInfo.resize(afterImageNum);

	for(int i = 0; i < _afterImageNum; i++) {
		_modelInfo[i] = new ModelInfo();

		_modelInfo[i]->use = false;
		_modelInfo[i]->remainTime = 0;

		_modelInfo[i]->modelHandle = ResourceServer::MV1LoadModel(keyName, modelName);
		_modelInfo[i]->alpha = 255;
	}
}

void AfterImage::SetAfterImage(MATRIX parentMatrix)
{
	for(int i = 0; i < _afterImageNum; i++) {
		if(_modelInfo[i]->use == false) {
			_modelInfo[i]->use = true;
			_modelInfo[i]->remainTime = 10;
			_modelInfo[i]->alpha = 1;

			auto color = MV1GetDifColorScale(_parentModelHandle);
			color.a = _modelInfo[i]->alpha;
			MV1SetDifColorScale(_modelInfo[i]->modelHandle, color);

			MV1SetMatrix(_modelInfo[i]->modelHandle, parentMatrix);

			break;
		}
	}
}

void AfterImage::Render()
{
	for(int i = 0; i < _afterImageNum; i++) {
		if(_modelInfo[i]->use == true) {
			MV1DrawModel(_modelInfo[i]->modelHandle);

			_modelInfo[i]->remainTime--;
			if(_modelInfo[i]->remainTime <= 0) {
				_modelInfo[i]->use = false;
			}
		}
	}
}

