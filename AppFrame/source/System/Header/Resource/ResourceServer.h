#pragma once
#include "dxlib.h"
#include <map>
#include <string>

class ResourceServer
{
public:
	ResourceServer();
	~ResourceServer();

	int LoadGraph(std::string handle_name);//�摜�n���h���̓ǂݍ���
	int LoadDivGraph(std::string handle_name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf); //�摜�n���h���̕����̓ǂݍ���
	int LoadSound(std::string sound_name);//�T�E���h�̓ǂݍ���
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

