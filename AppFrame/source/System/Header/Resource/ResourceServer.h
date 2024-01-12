#pragma once
#include "dxlib.h"
#include <map>
#include <string>
#include "EffekseerForDXLib.h"

class ResourceServer
{
public:

	static int LoadResource(const std::string resource_name, std::map<std::string, int>* server_name);

	static int LoadGraph(std::string handle_name);//�摜�n���h���̓ǂݍ���
	static int LoadDivGraph(std::string handle_name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf); //�摜�n���h���̕����̓ǂݍ���
	static int LoadEffekseerEffect(std::string handle_name);//�G�t�F�N�V�A�̃G�t�F�N�g�̓ǂݍ���
	static int MV1LoadModel(std::string model_name);//dxlib��.mv1�`���ɑΉ������R�c���f���̓ǂݍ���
	static int LoadSound(std::string sound_name);//�T�E���h�̓ǂݍ���
	static int SearchGraph(std::string name);//���������ɉ摜�����邩�����@�������ꍇ�摜�n���h���̒l��Ԃ�
	static void DeleteResource();//�S�Ẵ��\�[�X���폜

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