#pragma once
#include <vector>
#include "dxlib.h"
#include "Vector3D.h"
#include "appframe.h"
class bone
{
public:
	bone(int* Model, std::vector<int> list, int size);
	~bone();
	void SetMain(Vector3D* pos_list);
	void SetBoneDir(VECTOR world_dir_vec, VECTOR boon_pos, int target_frame, int parent_frame, MATRIX trans_mat, VECTOR dir_parent);;
	static const VECTOR _orign;

	VECTOR* _orignPos;

	VECTOR GetOrignPos(int index) { return _orignPos[index]; }
private:
	int* _model;
	int _listSize;
	VECTOR* _vecDirList;
	MATRIX* _transMatrixList;
	std::vector<int> _frameList;

	// ↓ ここから下は物理演算で使う変数や関数
public:
	Vector3D* _massPosList; // 質点の座標リスト
	Vector3D* _massAccelList; // 質点の速度リスト

	void UpdatePosAndAccel(double _elapsedTime);
	Vector3D ForceWorksToMassPoint(int i, Vector3D* posList, Vector3D* accelList); //質点に働く力を計算 F=ma
	bool Process();

	void PositionReset();

private:

	// 今回は髪の毛だけなので定数宣言しているがほかのに使うときは
	//変数として宣言し重力などの値をクラスを作るときの引数として入力するとできる
	//引数が多いので別クラスで作ったほうが良かったかも？
	static const float _massWeight;//質点の重量
	static const float _viscousResistance;//粘性抵抗
	static const float _gravity; //重力
	static const float _spring; //ばね定数
	static const double _processInterval; //処理の細分化の間隔

	static const float _naturalCorrectionFactor; //髪の毛の自然体の長さを出すときに使用する補正係数
	static const Vector3D _gravityDir;//重力の方向 

	int _massPointSize; //質点数
	std::vector<float> _springList; // ばね定数のリスト
	std::vector<float> _naturalList; // ばねの長さのリスト
};

