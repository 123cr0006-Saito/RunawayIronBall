//----------------------------------------------------------------------
// @filename PlaneEffectBase.h
// ＠date: 2024/01/30
// ＠author: saito ko
// @explanation
// 板ポリゴンを表示するエフェクトの基底クラス
//----------------------------------------------------------------------
#pragma once
#include "EffectBase.h"

	class PlaneEffectBase : public EffectBase
	{
	public:
		PlaneEffectBase(VECTOR pos, int sizeX, int* handle,int handleMax,int speed);
		virtual ~PlaneEffectBase();
		virtual bool Process()override;
		virtual bool Render()override;
		
	protected:
		static const unsigned short vertexOrder[6];// ポリゴンを描画する際の頂点の順番
		float _sizeX, _sizeY;//ポリゴンの横幅と縦幅
		int* _handle;// テクスチャハンドル
		int _speed;// アニメーションの再生速度
		int _currentTime;// 現在の時間
		int _animCount;// アニメーションのカウント
		int _animMax;// アニメーションの最大値
	};

