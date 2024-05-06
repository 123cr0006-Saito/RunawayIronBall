//----------------------------------------------------------------------
// @filename ObjectBase.cpp
// @date: 2024/03/07
// @author: Morozumi Hiroya
// 
// @explanation
// 当たり判定処理を行う全てのオブジェクトの基底クラス
// 当たり判定処理の管理に使用するCoollisionManagerクラスとCellクラスのインスタンスを保持する
// （CollisionManagerクラス：XZ平面に対して4分木空間分割を行い、当たり判定処理の回数を削減するためのクラス）
// （Cellクラス：CollisionManagerクラスに情報を登録するためのクラス）
//----------------------------------------------------------------------
#include "ObjectBase.h"
#include "Application/UtilMacro.h"

ObjectBase::ObjectBase()
{
	_collisionManager = CollisionManager::GetInstance();
	_cell = NEW Cell();
	_cell->_obj = this;
}

ObjectBase::~ObjectBase()
{
	_collisionManager = nullptr;
	SAFE_DELETE(_cell);
}
