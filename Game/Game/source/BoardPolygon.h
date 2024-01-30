#pragma once
#include "appframe.h"

namespace PlaneEffect {
	
	class BoardPolygon
	{
	public:
		//�@X�̒�������ɉ摜�̑傫������Y�𓱂��o���܂��B
		BoardPolygon(VECTOR pos, VECTOR dir, int sizeX, int* handle, int handleMax, int speed);//�����x�N�g���w���
		BoardPolygon(VECTOR pos, MATRIX matrix, int sizeX, int* handle,int handleMax, int speed);//�s��w���
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