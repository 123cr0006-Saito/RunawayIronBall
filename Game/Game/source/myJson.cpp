#include "myJson.h"
myJson::myJson(std::string fileName) {
	//�t�@�C�����J��
	std::ifstream file(fileName);
	if (!file) {
		//�t�@�C�����ǂݍ��߂Ȃ�����
		return;
	}
	//�t�@�C�����ǂݍ��߂��̂�json�N���X�Ɉړ�
	file >> _json;
	//�I�u�W�F�N�g�̐����i�[
	_size = _json.size();
};