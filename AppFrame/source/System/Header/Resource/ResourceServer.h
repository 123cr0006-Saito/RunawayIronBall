#pragma once
#include "dxlib.h"
#include <map>
#include <string>
#include <utility>
#include "EffekseerForDXLib.h"

class ResourceServer
{
public:

	static int LoadGraph(std::string handle_name);//�摜�n���h���̓ǂݍ���
	static int LoadDivGraph(std::string handle_name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf); //�摜�n���h���̕��������̓ǂݍ���
	//�摜�̕����ǂݍ��݁@���O�� �g���q�ƕ����Ă������� ���O�̓t�@�C���̂Ƃ���ł܂Ƃ߂ĕύX����΂��̂܂܎g����͂��ł��B
	static int LoadMultGraph(std::string handle_name, std::string extension, int AllNum, int* HandleBuf);
	static int LoadEffekseerEffect(std::string handle_name);//�G�t�F�N�V�A�̃G�t�F�N�g�̓ǂݍ���
	static int MV1LoadModel(std::string model_name);//dxlib��.mv1�`���ɑΉ������R�c���f���̓ǂݍ���
	static int LoadSound(std::string sound_name);//�T�E���h�̓ǂݍ���
	static int SearchGraph(std::string name);//���������ɉ摜�����邩�����@�������ꍇ�摜�n���h���̒l��Ԃ�


	static void DeleteResourceAll();//�S�Ẵ��\�[�X���폜

	//�����̉摜��ۑ����邽�߂̍\����
	static struct Div {
		int  AllNum;//�摜�̑S�̖̂���
		int* handle;//�摜��ۑ�����p�̕ϐ�   ���������I�ɕω����邽�߃|�C���^
	};

	static std::map<std::string, int> handle_server;//�摜��ۑ�����ϐ�
	static std::map<std::string, int> sound_server;//se�Ebgm��ۑ�����ϐ�
	static std::map<std::string, Div> div_server;//�����ɕ��������摜��ۑ�����ϐ�
	static std::map<std::string, Div> mult_server;//�����̉摜��ۑ�����ϐ�
	static std::map<std::string, int >effekseer_server;//�G�t�F�N�V�A�̃G�t�F�N�g��ۑ�����ϐ�
	static std::map<std::string, std::vector<int> >model_server;//dxlib�̃��f���n���h����ۑ�����ϐ�

};