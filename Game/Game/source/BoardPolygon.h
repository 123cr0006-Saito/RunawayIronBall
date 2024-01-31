#pragma once
#include "appframe.h"

namespace PlaneEffect {
	
	class BoardPolygon
	{
	public:
		//　Xの長さを基準に画像の大きさからYを導き出します。
		BoardPolygon(VECTOR pos, VECTOR dir, int sizeX, int* handle, int handleMax, int speed);//方向ベクトル指定版
		BoardPolygon(VECTOR pos, MATRIX matrix, int sizeX, int* handle,int handleMax, int speed);//行列指定版
		~BoardPolygon();
		bool Update();
		bool Render();
		bool GetFlag() { return _IsPlay; }
	protected:
		static const unsigned short vertexOrder[6];
		VERTEX3D polygon[4];
		int* _handle;
		int _speed;
		int _currentTime;
		int _animCount;
		int _animMax;
		bool _IsPlay;
	};
};