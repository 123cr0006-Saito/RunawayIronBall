#pragma once
#include "dxlib.h"
#include <map>
#include <string>
#include "EffekseerForDXLib.h"

class ResourceServer
{
public:

	static int LoadResource(const std::string resource_name, std::map<std::string, int>* server_name);

	static int LoadGraph(std::string handle_name);//画像ハンドルの読み込み
	static int LoadDivGraph(std::string handle_name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf); //画像ハンドルの複数の読み込み
	static int LoadEffekseerEffect(std::string handle_name);//エフェクシアのエフェクトの読み込み
	static int MV1LoadModel(std::string model_name);//dxlibの.mv1形式に対応した３Ｄモデルの読み込み
	static int LoadSound(std::string sound_name);//サウンドの読み込み
	static int SearchGraph(std::string name);//メモリ内に画像があるか検索　あった場合画像ハンドルの値を返す
	static void DeleteResource();//全てのリソースを削除

	static struct Div {
		int  AllNum;
		int* handle;
	};

	static std::map<std::string, int> handle_server;
	static std::map<std::string, int> sound_server;
	static std::map<std::string, Div> div_server;
	static std::map<std::string, int >effekseer_server;
	static std::map<std::string, int >model_server;

};