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

	static int LoadGraph(std::string key_name, std::string handle_name);//画像ハンドルの読み込み
	static int LoadDivGraph(std::string key_name, std::string handle_name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf); //画像ハンドルの複数分割の読み込み
	//画像の複数読み込み　名前は 拡張子と分けてください 名前はファイルのところでまとめて変更すればそのまま使えるはずです。
	static int LoadMultGraph(std::string key_name, std::string handle_name, std::string extension, int AllNum, int* HandleBuf);
	static int LoadEffekseerEffect(std::string key_name, std::string handle_name);//エフェクシアのエフェクトの読み込み
	static int MV1LoadModel(std::string key_name, std::string  model_name);//dxlibの.mv1形式に対応した３Ｄモデルの読み込み
	static int LoadSound(std::string key_name, std::string sound_name);//サウンドの読み込み
	
	static int SearchSingle(std::string search_key, TYPE resouceType);
	static bool SearchMult(std::string search_key,int* handle, int size);
	static std::pair<bool, int> DeleteSearchSingle(std::string search_key, std::map<std::string,int>* resourceMap);
	static std::pair<bool, ResourceServer::Mult> DeleteSearchMult(std::string search_key, std::map<std::string, Mult>* resourceMap);

	
	static bool Delete(std::string key, TYPE resouceType);

	static void DeleteResourceAll();//全てのリソースを削除

	static std::map<std::string, int> _handleMap;//画像を保存する変数
	static std::map<std::string, int> _soundMap;//se・bgmを保存する変数
	static std::map<std::string, Mult> _multMap;//複数の画像を保存する変数
	static std::map<std::string, int >_effekseerMap;//エフェクシアのエフェクトを保存する変数
	static std::map<std::string, std::vector<int> >_modelMap;//dxlibのモデルハンドルを保存する変数

};