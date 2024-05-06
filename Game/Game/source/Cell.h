//----------------------------------------------------------------------
// @filename Cell.h
// @date: 2024/03/07
// @author: Morozumi Hiroya
// 
// @explanation
// 当たり判定処理の管理を行うCollisionManagerクラスに情報を登録するためのクラス
// （CollisionManagerクラス：XZ平面に対して4分木空間分割を行い、当たり判定処理の回数を削減するためのクラス）
// CollisionManagerクラスで、分割したセグメントごとに属するCellの双方向リストを構築する
//----------------------------------------------------------------------
#pragma once
#include "ObjectBase.h"

// このCellインスタンスを保持しているオブジェクトの種類
enum OBJ_TYPE {
	NONE,
	PL,
	PL_IB,
	PL_IB_CHAIN,
	EN,
	EN_IB,
	BLDG,
	TWR,
	TWR_PRT,
};

class ObjectBase;
class Cell
{
public:
	Cell() {
		_segment = nullptr;
		_obj = nullptr;
		_objType = OBJ_TYPE::NONE;
		_prev = nullptr;
		_next = nullptr;
	}
	~Cell() {
		_segment = nullptr;
		_obj = nullptr;
		_prev = nullptr;
		_next = nullptr;
	}

	// 属しているセグメント（CollisionManagerクラス内で構築する双方向リストの先頭のダミーセル）
	Cell* _segment;
	// このCellインスタンスを保持しているオブジェクト
	ObjectBase* _obj;
	// このCellインスタンスを保持しているオブジェクトの種類
	OBJ_TYPE _objType;

	// 双方向リストの前後のセル
	Cell* _prev;
	Cell* _next;
};