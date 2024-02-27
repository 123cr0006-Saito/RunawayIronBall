#pragma once
#include "dxlib.h"
#include <map>
#include <string>
#include <utility>
#include "EffekseerForDXLib.h"

class ResourceServer
{
public:

	static enum class TYPE: int {
		Handle,
		Div,
		Mult,
		Efk,
		Model,
		Sound,
	};

	static int LoadGraph(const char* key_name, const char* handle_name);//画像ハンドルの読み込み
	static int LoadDivGraph(const char* key_name, const char* handle_name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf); //画像ハンドルの複数分割の読み込み
	//画像の複数読み込み　名前は 拡張子と分けてください 名前はファイルのところでまとめて変更すればそのまま使えるはずです。
	static int LoadMultGraph(const char* key_name, std::string handle_name, const char* extension, int AllNum, int* HandleBuf);
	static int LoadEffekseerEffect(const char* key_name, const char* handle_name);//エフェクシアのエフェクトの読み込み
	static int MV1LoadModel(const char* key_name, const char*  model_name);//dxlibの.mv1形式に対応した３Ｄモデルの読み込み
	static int LoadSound(const char* key_name, const char* sound_name);//サウンドの読み込み
	static int SearchGraph(const char* name);//メモリ内に画像があるか検索　あった場合画像ハンドルの値を返す
	template<typename T>
	static std::pair<bool, T> Search(const char* search_key, std::map<const char*,T>* resourceMap);
	static bool Delete(const char* key, TYPE resouceType);


	static void DeleteResourceAll();//全てのリソースを削除

	//複数の画像を保存するための構造体
	static struct Div {
		int  AllNum;//画像の全体の枚数
		int* handle;//画像を保存する用の変数   枚数が動的に変化するためポインタ
	};

	static std::map<const char*, int> handle_server;//画像を保存する変数
	static std::map<const char*, int> sound_server;//se・bgmを保存する変数
	static std::map<const char*, Div> div_server;//複数に分割した画像を保存する変数
	static std::map<const char*, Div> mult_server;//複数の画像を保存する変数
	static std::map<const char*, int >effekseer_server;//エフェクシアのエフェクトを保存する変数
	static std::map<const char*, std::vector<int> >model_server;//dxlibのモデルハンドルを保存する変数

};