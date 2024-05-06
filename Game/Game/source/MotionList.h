//----------------------------------------------------------------------
// @filename MotionList.h
// @date: 2024/02/20
// @author: Morozumi Hiroya
// @explanation
// 各キャラクターのモーションリストを管理するクラス
// AnimationManagerクラス及びFrameDataクラスの初期化処理で必要な情報を管理する
// "Data/MotionList/MotionList_〇〇.csv" から、各キャラクターのモーション一覧を読み込む（○○の部分にはキャラクター名が入る）
//----------------------------------------------------------------------
#pragma once
#include <vector>
#include <string>
#include <map>

// モーション名とループ回数のペア
typedef std::pair<std::string, int> MotionNamePair;
typedef std::string CHARA_NAME;

class MotionList
{
public:
	// モーションリストの読み込み
	// @param charaName: キャラクター名
	// @param fileName: モーションリストのファイル名
	static void Load(CHARA_NAME charaName, std::string fileName);
	// モーションリストをすべて削除する
	static void DeleteAll();
	// 指定したキャラクターのモーションリストを取得する
	// @param charaName: キャラクター名
	static std::vector<MotionNamePair>* GetMotionList(CHARA_NAME charaName);
private:
	// 全キャラクターのモーションリスト
	// @key: キャラクター名
	// @value: モーションリスト
	static std::map<CHARA_NAME, std::vector<MotionNamePair>> _allCharaMotionList;
};