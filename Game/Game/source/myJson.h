#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
class myJson
{
public:
	//�ǂݍ��ݗp
	myJson(std::string fileName);

	nlohmann::json _json;
	int _size;
};

