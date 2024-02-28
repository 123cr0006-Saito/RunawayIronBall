#pragma once
#include "appframe.h"
namespace PlaneEffect {

	class PlaneEffectBase
	{
	public:
		PlaneEffectBase(VECTOR pos, int sizeX, int* handle,int handleMax,int speed);
		virtual ~PlaneEffectBase();
		virtual bool Update();
		virtual bool Render();
		bool GetFlag() { return _IsPlay; }
	protected:
		static const unsigned short vertexOrder[6];
		float _sizeX, _sizeY;
		int* _handle;
		int _speed;
		int _currentTime;
		int _animCount;
		int _animMax;
		bool _IsPlay;
	};
}
