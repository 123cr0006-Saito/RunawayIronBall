//----------------------------------------------------------------------
// @filename myJson.h
// ��date: 2024/01/18
// ��author: saito ko
// @explanation
// json�t�@�C����ǂݍ��ރN���X
//----------------------------------------------------------------------
#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
class myJson
{
public:
	//�ǂݍ��ݗp
	myJson(std::string fileName);
	~myJson();

	nlohmann::json _json;
	int _size;
};

