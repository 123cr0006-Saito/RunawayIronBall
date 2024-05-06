//----------------------------------------------------------------------
// @filename ModelColor.cpp
// @date: 2024/02/15
// @author: Morozumi Hiroya
// @explanation
// モデルの白点滅処理を行うクラス
//----------------------------------------------------------------------
#include "ModelColor.h"

ModelColor::ModelColor()
{
	_modelHandle = -1;
	_flickerEmissiveColor = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	_flickerTextureHandle = -1;
}

ModelColor::~ModelColor()
{
	for (auto itr = _defaultMaterial.begin(); itr != _defaultMaterial.end(); ++itr)
	{
		delete (*itr);
		(*itr) = nullptr;
	}
	_defaultMaterial.clear();
}

// 初期化処理
void ModelColor::Init(int modelHandle)
{
	_modelHandle = modelHandle;

	// モデルのマテリアル情報を取得
	for (int i = 0; i < MV1GetMaterialNum(_modelHandle); i++)
	{
		MATERIAL* material = NEW MATERIAL();
		material->index = i;
		material->textureHandle = MV1GetTextureGraphHandle(_modelHandle, i);
		material->emiColor = MV1GetMaterialEmiColor(_modelHandle, i);

		_defaultMaterial.push_back(material);
	}

	_flickerEmissiveColor = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	_flickerTextureHandle = ResourceServer::LoadGraph("GirlTexWhite","res/Character/cg_player_girl/FlickerTexture.png");
}

// 白点滅処理
// 以下の処理を繰り返して白点滅を行う
// 1.テクスチャを白い画像に差し替え、エミッシブカラーを白にする
// 2.元に戻す
// @param activateFlicker: 白点滅状態の変更（true:1の状態にする, false:2の状態にする）
void ModelColor::ChangeFlickerTexture(bool activateFlicker)
{
	for (auto itr = _defaultMaterial.begin(); itr != _defaultMaterial.end(); ++itr)
	{
		int nextTextureHandle;
		COLOR_F nextColor;
		// 有効化
		if (activateFlicker) {
			// テクスチャとエミッシブカラーを白に差し替え
			nextTextureHandle = _flickerTextureHandle;
			nextColor = _flickerEmissiveColor;
		}
		// 無効化
		else {
			// テクスチャとエミッシブカラーを元に戻す
			nextTextureHandle = (*itr)->textureHandle;
			nextColor = (*itr)->emiColor;
		}
		// モデルのマテリアルにテクスチャとエミッシブカラーを設定する
		MV1SetTextureGraphHandle(_modelHandle, (*itr)->index, nextTextureHandle, FALSE);
		MV1SetMaterialEmiColor(_modelHandle, (*itr)->index, nextColor);
	}
}
