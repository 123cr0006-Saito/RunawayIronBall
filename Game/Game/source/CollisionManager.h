//----------------------------------------------------------------------
// @filename CollisionManager.h
// @date: 2024/03/07
// @author: Morozumi Hiroya
// 
// @explanation
// 当たり判定処理の管理を行うクラス
// XZ平面に対して4分木空間分割を行い、当たり判定処理の回数を削減する
// 
// Cellクラスを用いて、オブジェクトを管理する
// CellのOBJ_TYPEに関しては、ObjectBaseクラスの派生クラスのInit関数で設定する
// CellのOBJ_TYPEによりObjectBaseクラスのどの派生クラスかの判別がつくので、ObjectBaseクラスから各派生クラスへのダウンキャストはstatic_castで行う
// 
// @reference
// 参考サイト
// https://qiita.com/mogamoga1337/items/a1060d531b70c32a8ade
// http://marupeke296.com/COL_2D_No8_QuadTree.html
// http://marupeke296.com/COL_2D_No9_QuadTree_Imp.html
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "Cell.h"
#include "ObjectBase.h"

class CharacterBase;
class Player;
class IronBall;
class EnemyBase;
class BuildingBase;
class Tower;
class TowerParts;

// XZ平面上で4分木空間分割を行い、当たり判定を行うクラス
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	// インスタンスを取得
	static CollisionManager* GetInstance() { return _instance; }

	// 初期化処理
	void Init();
	// 更新処理
	void Process();

	// セルをツリーへ追加、更新
	void UpdateCell(Cell* cell);
	// セルの削除予約
	void ReserveRemovementCell(Cell* cell);

	// ツリーとリストのクリア
	// _tree, _colList, _reserveRemovementListをクリアする
	void ClearTreeAndList();

	// デバッグ情報の表示
	// セグメントの区切りをワールド空間に描画
	void DrawSegment();
	// セルが属するセグメントのインデックスをそのセルを保持するオブジェクトの座標に描画
	void DrawSegmentIndex();

private:
	// 2つの頂点からセルが属するツリーのインデックスを計算
	unsigned int CalcTreeIndex(VECTOR pos1, VECTOR pos2);
	// 指定された座標が属するセグメントのインデックスを計算（最小単位まで分割したセグメント）
	unsigned int CalcSegment(VECTOR pos);
	// 入力値を2進数で一つ飛ばしに分割する
	// 例 : 1101 -> 01 01 00 01
	unsigned int SeparateBit(unsigned int n);

	// ツリーにセルを追加
	// ダミーセルの次にセルを追加する
	void InsertCellIntoTree(unsigned int treeIndex, Cell* cell);

	// ツリーからセルを削除
	void RemoveCell(Cell* cell);
	// 削除予約リストにあるセルを削除
	void RemoveCellFromReserveList();

	// 当たり判定リストを作成する
	void CreateColList(unsigned int treeIndex, std::list<Cell*>& colStack);
	// 当たり判定処理
	void CheckColList();

	// --------------------------------------------------------------------------------------------
	// 当たり判定処理
	void CheckHit(Player* player, EnemyBase* enemy);
	void CheckHit(Player* player, BuildingBase* building);
	void CheckHit(Player* player, Tower* tower);

	void CheckHitIbAndEn(IronBall* ironBall, EnemyBase* enemy);
	void CheckHitIbAndBldg(IronBall* ironBall, BuildingBase* building);
	void CheckHitIbAndTwr(IronBall* ironBall, Tower* tower);

	void CheckHitChAndEn(IronBall* ironBall, EnemyBase* enemy);
	void CheckHitChAndBldg(IronBall* ironBall, BuildingBase* building);
	void CheckHitChAndTwr(IronBall* ironBall, Tower* tower);

	void CheckHit(EnemyBase* enemy1, EnemyBase* enemy2);
	void CheckHit(EnemyBase* enemy, BuildingBase* building);
	void CheckHit(EnemyBase* enemy, Tower* tower);
	void CheckHit(EnemyBase* enemy, TowerParts* towerParts);

	void CheckHit(BuildingBase* building, TowerParts* towerParts);
	// --------------------------------------------------------------------------------------------

	// インスタンス
	static CollisionManager* _instance;

	// ワールドの原点からのオフセット
	float _offsetX;
	float _offsetZ;

	// 1辺あたりのセグメント数
	int _segmentNumPerSide;
	// 最小単位まで分割したセグメントの1辺の長さ
	float _segmentLength;

	// ツリーのサイズ
	// (pow(4, 分割回数 + 1) - 1) / 3
	int _treeSize;
	// ツリー（線形4分木）
	// 各セグメントに属しているオブジェクトを登録する
	// 要素ごとに双方向リストを構築する
	// 各要素の先頭にはダミーのセルを格納し、それより後ろにはオブジェクトを格納する
	std::vector<Cell*> _tree;

	// 当たり判定リスト
	// 当たり判定を行う2つセルのセルをペアで格納する
	std::list<std::pair<Cell*, Cell*>> _colList;

	// 削除予約リスト
	std::list<Cell*> _reserveRemovementList;
};