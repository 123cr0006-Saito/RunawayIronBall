#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
class myJson
{
public:
	//ì«Ç›çûÇ›óp
	myJson(std::string fileName);

	nlohmann::json _json;
	int _size;
};

