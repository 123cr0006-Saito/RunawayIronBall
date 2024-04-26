//----------------------------------------------------------------------
// @filename myJson.h
// @date: 2024/01/18
// @author: saito ko
// @explanation
// jsonファイルを読み込むクラス
//----------------------------------------------------------------------
#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
class myJson
{
public:
	//読み込み用
	myJson(std::string fileName);
	~myJson();

	nlohmann::json _json;// jsonファイルの内容
	int _size;// jsonファイルのサイズ
};

