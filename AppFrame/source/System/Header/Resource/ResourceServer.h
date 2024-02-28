#pragma once
#include "dxlib.h"
#include <map>
#include <string>
#include <utility>
#include "EffekseerForDXLib.h"

class ResourceServer
{
public:

	static enum class TYPE: int {
		Handle,
		Mult,
		Efk,
		Model,
		Sound,
	};

	//�����̉摜��ۑ����邽�߂̍\����
	static struct Mult {
		int  AllNum;//�摜�̑S�̖̂���
		int* handle;//�摜��ۑ�����p�̕ϐ�   ���������I�ɕω����邽�߃|�C���^
	};

	static int LoadGraph(const char* key_name, const char* handle_name);//�摜�n���h���̓ǂݍ���
	static int LoadDivGraph(const char* key_name, const char* handle_name, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf); //�摜�n���h���̕��������̓ǂݍ���
	//�摜�̕����ǂݍ��݁@���O�� �g���q�ƕ����Ă������� ���O�̓t�@�C���̂Ƃ���ł܂Ƃ߂ĕύX����΂��̂܂܎g����͂��ł��B
	static int LoadMultGraph(const char* key_name, std::string handle_name, const char* extension, int AllNum, int* HandleBuf);
	static int LoadEffekseerEffect(const char* key_name, const char* handle_name);//�G�t�F�N�V�A�̃G�t�F�N�g�̓ǂݍ���
	static int MV1LoadModel(const char* key_name, const char*  model_name);//dxlib��.mv1�`���ɑΉ������R�c���f���̓ǂݍ���
	static int LoadSound(const char* key_name, const char* sound_name);//�T�E���h�̓ǂݍ���
	
	static int SearchSingle(const char* search_key, TYPE resouceType);
	static bool SearchMult(const char* search_key,int* handle, int size);
	static std::pair<bool, int> DeleteSearchSingle(const char* search_key, std::map<const char*,int>* resourceMap);
	static std::pair<bool, ResourceServer::Mult> DeleteSearchMult(const char* search_key, std::map<const char*, Mult>* resourceMap);

	
	static bool Delete(const char* key, TYPE resouceType);

	static void DeleteResourceAll();//�S�Ẵ��\�[�X���폜

	static std::map<const char*, int> _handleMap;//�摜��ۑ�����ϐ�
	static std::map<const char*, int> _soundMap;//se�Ebgm��ۑ�����ϐ�
	static std::map<const char*, Mult> _multMap;//�����̉摜��ۑ�����ϐ�
	static std::map<const char*, int >_effekseerMap;//�G�t�F�N�V�A�̃G�t�F�N�g��ۑ�����ϐ�
	static std::map<const char*, std::vector<int> >_modelMap;//dxlib�̃��f���n���h����ۑ�����ϐ�

};