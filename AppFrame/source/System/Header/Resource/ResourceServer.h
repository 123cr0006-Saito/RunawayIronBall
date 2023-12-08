#pragma once
#include "dxlib.h"
#include <map>
#include <string>

class ResourceServer
{
public:
	ResourceServer();
	~ResourceServer();

	int LoadGraph(std::string handle_name);//画像ハンドルの読み込み
	int LoadDivGraph(std::string handle_name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf); //画像ハンドルの複数の読み込み
	int LoadSound(std::string sound_name);//サウンドの読み込み
	int SearchGraph(std::string name);

private:
	struct Div{
		int  AllNum;
		int* handle;
	};
	std::map<std::string, int> handle_server;
	std::map<std::string, int> sound_server;
	std::map<std::string, Div> div_server;
};

