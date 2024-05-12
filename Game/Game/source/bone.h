//----------------------------------------------------------------------
// @filename bone.h
// @date: 2023/12/30
// @author: saito ko
// @explanation
// キャラクターの髪の毛やリボンなどをboneを物理演算で動かすためのクラス
//----------------------------------------------------------------------
#pragma once
#include <vector>
#include "dxlib.h"
#include "appframe.h"
#include "myJson.h"

#define EULER 0
//#define RUNGE_KUTTA 1

class bone
{
public:
	bone(int* Model, std::vector<int> list, int size, std::string jsonFileName);
	~bone();
	void SetMain(Vector3D* pos_list);//boneの位置をセットする
	void SetBoneDir(Vector3D world_dir_vec, Vector3D boon_pos, int target_frame, int parent_frame, MATRIX trans_mat, Vector3D dir_parent);;
	void SetDebugDraw() { _isTrans = 1 - _isTrans; };//;透過するかどうか
	void DebugProcess(int transNum);//透過フラグが立っていたらboneのフレームを透過　透過したいフレームと連携していなかった場合引数で指定
	void DebugRender();//透過されていた場合デバッグ表示

	static const Vector3D _orign;

	Vector3D* _orignPos;

	Vector3D GetOrignPos(int index) { return _orignPos[index]; }//初期地点の座標を得る
private:
	int* _model;
	int _listSize;
	Vector3D* _vecDirList;//初期時点でのボーンから見る親の方向
	MATRIX* _transMatrixList;//座標変換行列のリスト
	std::vector<int> _frameList;//フレームの番号リスト

	//デバッグ表示
	bool _isTrans;//表示するかしないか
	bool _oldIsTrans;

	// ↓ ここから下は物理演算で使う変数や関数
public:
	void SetGravity(std::string end, std::string start);
	void UpdatePosAndAccelByEuler(double _elapsedTime);
	void UpdatePosAndAccelByRungeKutta(double _elapsedTime);
	Vector3D ForceWorksToMassPoint(int i, Vector3D* posList, Vector3D* accelList); //質点に働く力を計算 F=ma
	bool Process();

	void PositionReset();

	Vector3D* _massPosList; // 質点の座標リスト
	Vector3D* _massAccelList; // 質点の速度リスト

private:

	// 今回は髪の毛だけなので定数宣言しているがほかのに使うときは
	//変数として宣言し重力などの値をクラスを作るときの引数として入力するとできる
	//引数が多いので別クラスで作ったほうが良かったかも？
	float* _massWeight;//質点の重量
	float* _viscousResistance;//粘性抵抗
	float* _gravity; //重力
	float* _spring; //ばね定数
	static const double _processInterval; //処理の細分化の間隔

	float* _naturalCorrectionFactor; //髪の毛の自然体の長さを出すときに使用する補正係数
	Vector3D _gravityDir;//重力の方向 

	int _massPointSize; //質点数
	std::vector<float> _springList; // ばね定数のリスト
	std::vector<float> _naturalList; // ばねの長さのリスト
};

