#include "BoardPolygon.h"

const unsigned short PlaneEffect::BoardPolygon::vertexOrder[6] = { 0,1,2,2,1,3};

PlaneEffect::BoardPolygon::BoardPolygon(VECTOR pos, VECTOR dir, int sizeX, int* handle, int handleMax, int speed) {
	_animCount = 0;
	_animMax = handleMax;
	_handle = new int[_animMax];
	std::swap(_handle,handle);
	_speed = speed;
	_currentTime = GetNowCount();
	_IsPlay = true;

	//設定したサイズともともとの画像サイズから比率でサイズを割り出す
	int x, y;
	GetGraphSize(_handle[0], &x, &y);
	int sizeY = y * sizeX / x;
	//直交座標系を求める
	VECTOR PolyVecX = VNorm(VCross(dir, VGet(0,-1, 0)));
	VECTOR PolyVecY = VNorm(VCross(dir,PolyVecX));

	VECTOR VecSize[4] = {
		VAdd(VAdd(VScale(PolyVecX,-sizeX / 2),VScale(PolyVecY, sizeY / 2)),pos),
		VAdd(VAdd(VScale(PolyVecX, sizeX / 2),VScale(PolyVecY, sizeY / 2)),pos),
		VAdd(VAdd(VScale(PolyVecX,-sizeX / 2),VScale(PolyVecY,-sizeY / 2)),pos),
		VAdd(VAdd(VScale(PolyVecX, sizeX / 2),VScale(PolyVecY,-sizeY / 2)),pos)
	};
	//uvリスト
	float uvList[4][2] = {
	{0.0 ,0.0 },
	{1.0f,0.0 },
	{0.0 ,1.0f},
	{1.0f,1.0f}
	};
	//初期化
	for (int i = 0; i < 4; i++) {
		polygon[i].pos = VecSize[i];
		//向かっている方向の逆なので*-1
		polygon[i].norm = VScale(dir,-1);
		polygon[i].dif = GetColorU8(255, 255, 255, 255);
		polygon[i].spc = GetColorU8(0, 0, 0, 0);
		polygon[i].u = uvList[i][0];
		polygon[i].v = uvList[i][1];
		polygon[i].su = 0.0f;
		polygon[i].sv = 0.0f;
	}
	
};

PlaneEffect::BoardPolygon::BoardPolygon(VECTOR pos, MATRIX matrix, int sizeX, int* handle,int handleMax, int speed) {
	_animCount = 0;
	_animMax = handleMax;
	_handle = new int[_animMax];
	std::swap(_handle, handle);
	_speed = speed;
	_currentTime = GetNowCount();
	_IsPlay = true;

	//設定したサイズともともとの画像サイズから比率でサイズを割り出す
	int x, y;
	GetGraphSize(_handle[0], &x, &y);
	float sizeY = y * sizeX / x;

	VECTOR VecSize[4] = {
	VAdd(pos,VTransform(VGet( -sizeX / 2, sizeY / 2,0), matrix)),
	VAdd(pos,VTransform(VGet( sizeX / 2,  sizeY / 2,0), matrix)),
	VAdd(pos,VTransform(VGet( -sizeX / 2,  -sizeY / 2,0), matrix)),
	VAdd(pos,VTransform(VGet( sizeX / 2, -sizeY / 2,0), matrix))
	};
	//uvリスト
	float uvList[4][2] = {
		{0.0 ,0.0 },
		{1.0f,0.0 },
		{0.0 ,1.0f},
		{1.0f,1.0f}
	};

	//マトリックスからフォワードベクトルを取り出す
	VECTOR norm = Math::MatrixToVector(matrix,2);
	//初期化
	for (int i = 0; i < 4; i++) {
		polygon[i].pos = VecSize[i];
		polygon[i].norm = norm;
		polygon[i].dif = GetColorU8(255, 255, 255, 255);
		polygon[i].spc = GetColorU8(0, 0, 0, 0);
		polygon[i].u = uvList[i][0];
		polygon[i].v = uvList[i][1];
		polygon[i].su = 0.0f;
		polygon[i].sv = 0.0f;
	}

};

PlaneEffect::BoardPolygon::~BoardPolygon() {
	
	
};

bool PlaneEffect::BoardPolygon::Update(){
	if (GetNowCount() - _currentTime >= _speed) {
		_currentTime = GetNowCount();
		_animCount++;
		if (_animCount > _animMax) {
			_IsPlay = false;
		}
	}
	return true;
};

bool PlaneEffect::BoardPolygon::Render(){
	if (_IsPlay) {
		DrawPolygonIndexed3D(polygon, 4, vertexOrder, 2, _handle[_animCount], true);
	}
	return true;
};