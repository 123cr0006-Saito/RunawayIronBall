#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
class myJson
{
public:
	//�ǂݍ��ݗp
	myJson(std::string fileName);

protected:
	nlohmann::json json;
	int _size;
};