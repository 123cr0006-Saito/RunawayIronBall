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
		static const unsigned short vertexOrder[6];
		float _sizeX, _sizeY;
		int* _handle;
		int _speed;
		int _currentTime;
		int _animCount;
		int _animMax;
	};

