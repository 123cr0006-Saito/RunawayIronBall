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
		Mult,
		Efk,
		Model,
		Sound,
	};

	//複数の画像を保存するための構造体
	static struct Mult {
		int  AllNum;//画像の全体の枚数
		int* handle;//画像を保存する用の変数   枚数が動的に変化するためポインタ
	};

	static int LoadGraph(const char* key_name, const char* handle_name);//画像ハンドルの読み込み
	static int LoadDivGraph(const char* key_name, const char* handle_name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf); //画像ハンドルの複数分割の読み込み
	//画像の複数読み込み　名前は 拡張子と分けてください 名前はファイルのところでまとめて変更すればそのまま使えるはずです。
	static int LoadMultGraph(const char* key_name, std::string handle_name, const char* extension, int AllNum, int* HandleBuf);
	static int LoadEffekseerEffect(const char* key_name, const char* handle_name);//エフェクシアのエフェクトの読み込み
	static int MV1LoadModel(const char* key_name, const char*  model_name);//dxlibの.mv1形式に対応した３Ｄモデルの読み込み
	static int LoadSound(const char* key_name, const char* sound_name);//サウンドの読み込み
	
	static int SearchSingle(const char* search_key, TYPE resouceType);
	static bool SearchMult(const char* search_key,int* handle, int size);
	static std::pair<bool, int> DeleteSearchSingle(const char* search_key, std::map<const char*,int>* resourceMap);
	static std::pair<bool, ResourceServer::Mult> DeleteSearchMult(const char* search_key, std::map<const char*, Mult>* resourceMap);

	
	static bool Delete(const char* key, TYPE resouceType);

	static void DeleteResourceAll();//全てのリソースを削除

	static std::map<const char*, int> _handleMap;//画像を保存する変数
	static std::map<const char*, int> _soundMap;//se・bgmを保存する変数
	static std::map<const char*, Mult> _multMap;//複数の画像を保存する変数
	static std::map<const char*, int >_effekseerMap;//エフェクシアのエフェクトを保存する変数
	static std::map<const char*, std::vector<int> >_modelMap;//dxlibのモデルハンドルを保存する変数

};