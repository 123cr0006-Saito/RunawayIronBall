#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
class myJson
{
public:
	//読み込み用
	myJson(std::string fileName);

	nlohmann::json _json;
	int _size;
};

