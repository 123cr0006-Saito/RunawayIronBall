//----------------------------------------------------------------------
// @filename ModelColor.h
// @date: 2024/02/15
// @author: Morozumi Hiroya
// @explanation
// モデルの白点滅処理を行うクラス
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"

class ModelColor
{
public:
	ModelColor();
	~ModelColor();

	// 初期化処理
	void Init(int modelHandle);

	// 白点滅処理
	// 以下の処理を繰り返して白点滅を行う
	// 1.テクスチャを白い画像に差し替え、エミッシブカラーを白にする
	// 2.元に戻す
	// @param activateFlicker: 白点滅状態の変更（true:1の状態にする, false:2の状態にする）
	void ChangeFlickerTexture(bool activateFlicker);

private:
	// モデルハンドル
	int _modelHandle;

	// マテリアル情報
	struct MATERIAL
	{
		// マテリアルのインデックス番号
		int index;
		// テクスチャ画像ハンドル
		int textureHandle;
		// エミッシブカラー
		COLOR_F emiColor;
	};
	std::vector<MATERIAL*> _defaultMaterial;

	// 白点滅用のテクスチャ画像ハンドル
	int _flickerTextureHandle;
	// 白点滅用のエミッシブカラー
	COLOR_F _flickerEmissiveColor;
};